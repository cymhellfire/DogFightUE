// Dog Fight Game Code By CYM.


#include "Actors/Components/ActorTeleportComponent.h"
#include "Actors/Vfx/VfxBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UActorTeleportComponent::UActorTeleportComponent()
{
	CurrentTeleportPhase = EActorTeleportPhase::ATP_Idle;
	TeleportCooldown = 3.5f;
	TeleportCountdown = 3.5f;
	ZAxisCorrectionOffset = 200.f;
}

void UActorTeleportComponent::MulticastStartTeleport_Implementation(const FVector& Destination)
{
	StartTeleport(Destination);
}

void UActorTeleportComponent::StartTeleport(FVector Destination)
{
	if (CurrentTeleportPhase != EActorTeleportPhase::ATP_Idle)
	{
		return;
	}

	CacheDestination = Destination;

	if (GetNetMode() != NM_Client)
	{
		// Spawn origin vfx
		USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			SkeletalMeshComponent = Character->GetMesh();
		}
		const FVector VfxLocation = IsValid(SkeletalMeshComponent) ? SkeletalMeshComponent->GetComponentLocation() : GetOwner()->GetActorLocation();

		if (IsValid(TeleportOriginVfx))
		{
			GetWorld()->SpawnActor<AVfxBase>(TeleportOriginVfx, VfxLocation, FRotator::ZeroRotator);
		}
	}

	if (TeleportCountdown > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &UActorTeleportComponent::OnTeleportCountdownExpired, TeleportCountdown);
	}
	else
	{
		OnTeleportCountdownExpired();
	}

	OnTeleportStarted.Broadcast(this);

	CurrentTeleportPhase = EActorTeleportPhase::ATP_Countdown;
}

void UActorTeleportComponent::OnTeleportCountdownExpired()
{
	UWorld* CurrentWorld = GetWorld();
	if (TeleportTimerHandle.IsValid())
	{
		CurrentWorld->GetTimerManager().ClearTimer(TeleportTimerHandle);
	}

	if (GetNetMode() != NM_Client)
	{
		FVector DestVfxPos = CacheDestination;
		// Correct Z coordinate by sweep collision
		if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(GetOwner()->GetRootComponent()))
		{
			static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingle"));
			FCollisionQueryParams QueryParams(CapsuleTraceSingleName, false);
			QueryParams.AddIgnoredActor(GetOwner());
			FHitResult HitResult;
			FVector StartPos = FVector(CacheDestination.X, CacheDestination.Y, CacheDestination.Z + ZAxisCorrectionOffset);
			FVector EndPos = FVector(CacheDestination.X, CacheDestination.Y, CacheDestination.Z - ZAxisCorrectionOffset);
			if (CurrentWorld->SweepSingleByProfile(HitResult, StartPos, EndPos, FQuat::Identity, CapsuleComponent->GetCollisionProfileName(),
				FCollisionShape::MakeCapsule(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight()), QueryParams))
			{
				CacheDestination.SetComponentForAxis(EAxis::Z, HitResult.ImpactPoint.Z + CapsuleComponent->GetScaledCapsuleHalfHeight());
				DestVfxPos.SetComponentForAxis(EAxis::Z, HitResult.ImpactPoint.Z);
			}
		}

		// Spawn destination vfx
		if (IsValid(TeleportDestinationVfx))
		{
			CurrentWorld->SpawnActor<AVfxBase>(TeleportDestinationVfx, DestVfxPos, FRotator::ZeroRotator);
		}

		// Do teleport
		if (AActor* Owner = GetOwner())
		{
			FTransform NewTrans(Owner->GetTransform());
			NewTrans.SetLocation(CacheDestination);
			Owner->SetActorTransform(NewTrans, false, nullptr, ETeleportType::ResetPhysics);
		}
	}

	OnArrivedDestination.Broadcast(this);

	CurrentTeleportPhase = EActorTeleportPhase::ATP_Cooldown;

	if (TeleportCooldown > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &UActorTeleportComponent::OnTeleportCooldownExpired, TeleportCooldown);
	}
	else
	{
		OnTeleportCooldownExpired();
	}
}

void UActorTeleportComponent::OnTeleportCooldownExpired()
{
	if (TeleportTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);
	}

	OnTeleportFinished.Broadcast(this);

	CurrentTeleportPhase = EActorTeleportPhase::ATP_Idle;
}

