// Dog Fight Game Code By CYM.


#include "InstructionSpawnActor.h"

#include "CustomizableCard.h"

UInstructionSpawnActor::UInstructionSpawnActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("SpawnActor");
	bAutoCalculateRotation = true;
}

void UInstructionSpawnActor::BeginDestroy()
{
	Super::BeginDestroy();

	// Destroy all spawned actors
	if (ActorDestructionType == EActorDestructionType::ADT_AutoWithCard)
	{
		for (int32 Index = SpawnedActors.Num() - 1; Index >= 0; --Index)
		{
			AActor* CurrentActor = SpawnedActors[Index];
			SpawnedActors.Remove(CurrentActor);
			CurrentActor->ConditionalBeginDestroy();
		}
		SpawnedActors.Empty();
	}
}

bool UInstructionSpawnActor::HandleActorTarget(AActor* Target)
{
	const bool Result = Super::HandleActorTarget(Target);
	if (!Result)
	{
		return false;
	}

	// Skip once broadcast since the jump invoke below
	bSkipOneBroadcast = true;

	HandlePositionTarget(Target->GetActorLocation());
	return true;
}

void UInstructionSpawnActor::HandlePositionTarget(FVector Position)
{
	Super::HandlePositionTarget(Position);

	if (bForceUseDirectionSpawn)
	{
		bSkipOneBroadcast = true;
		HandleDirectionTarget(Position - GetOwnerControlledPawn()->GetActorLocation());
		return;
	}

	// Calculate spawn rotation based on settings
	FRotator SpawnRotation;
	if (bAutoCalculateRotation)
	{
		const FVector SpawnDirection = Position - GetOwnerControlledPawn()->GetActorLocation();
		SpawnRotation = SpawnDirection.Rotation();
	}
	else
	{
		SpawnRotation = InitialRotator;
	}

	// Spawn actor
	SpawnActorAt(Position, SpawnRotation);
}

void UInstructionSpawnActor::HandleDirectionTarget(FVector Direction)
{
	Super::HandleDirectionTarget(Direction);

	Direction.Normalize();
	// Calculate spawn location
	FVector SpawnLocation = GetOwnerControlledPawn()->GetActorLocation() + Direction * SpawnDistance;
	FRotator SpawnRotation = bAutoCalculateRotation ? Direction.Rotation() : InitialRotator;
	SpawnLocation += SpawnRotation.RotateVector(SpawnOffset);

	// Spawn actor
	SpawnActorAt(SpawnLocation, SpawnRotation);
}

void UInstructionSpawnActor::OnHandledAllTarget()
{
	// Attach all spawned actors if necessary
	if (bAttachToCardUser)
	{
		if (AttachDelay > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(DelayAttachTimerHandle, this, &UInstructionSpawnActor::OnDelayAttachTimerExpired, AttachDelay);
		}
		else
		{
			AttachActorsToOwner();
		}

		// Set auto detach timer
		if (bAutoDetach)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoDetachTimerHandle, this, &UInstructionSpawnActor::OnAutoDetachTimerExpired, AutoDetachDelay + AttachDelay);
		}
	}
}

void UInstructionSpawnActor::SpawnActorAt(FVector SpawnLocation, FRotator SpawnRotator)
{
	if (!IsValid(ActorClass))
	{
		UE_LOG(LogCustomizableCard, Error, TEXT("Invalid actor class to spawn."));
		return;
	}

	if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnLocation, SpawnRotator))
	{
		if (ActorDestructionType == EActorDestructionType::ADT_Lifespan)
		{
			NewActor->SetLifeSpan(Lifespan);
		}

		// Record new actor to list
		SpawnedActors.Add(NewActor);
	}
}

void UInstructionSpawnActor::AttachActorsToOwner()
{
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::KeepWorldTransform;
	switch(AttachType)
	{
	case EAttachRulesType::ART_KeepRelative:
		AttachRules = FAttachmentTransformRules::KeepRelativeTransform;
		break;
	case EAttachRulesType::ART_SnapToTarget:
		AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		break;
	case EAttachRulesType::ART_KeepWorld:
	default: ;
	}

	// Attach spawned actors to owner
	APawn* OwnerPawn = GetOwnerControlledPawn();
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
	{
		if (USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh())
		{
			for (AActor* MyActor : SpawnedActors)
			{
				MyActor->AttachToComponent(MeshComponent, AttachRules, AttachSocketName);
			}
		}
	}
	else
	{
		for (AActor* MyActor : SpawnedActors)
		{
			MyActor->AttachToActor(OwnerPawn, AttachRules, AttachSocketName);
		}
	}
}

void UInstructionSpawnActor::OnDelayAttachTimerExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayAttachTimerHandle);

	AttachActorsToOwner();
}

void UInstructionSpawnActor::OnAutoDetachTimerExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(AutoDetachTimerHandle);

	for (AActor* MyActor : SpawnedActors)
	{
		MyActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}
