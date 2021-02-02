// Dog Fight Game Code By CYM.


#include "Animation/AnimNotify_SpawnActor.h"

UAnimNotify_SpawnActor::UAnimNotify_SpawnActor()
	: Super()
{
	bAutoCalculateRotation = true;
	Lifespan = 1.f;
}

void UAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// Record owner
	OwnerComponent = MeshComp;

	// Get the forward vector
	const FVector MeshForward = MeshComp->GetOwner()->GetActorForwardVector();

	FVector SpawnLocation = MeshComp->GetOwner()->GetActorLocation() + MeshForward * SpawnDistance;
	FRotator SpawnRotator = bAutoCalculateRotation ? MeshForward.Rotation() : InitialRotation;
	SpawnLocation += SpawnRotator.RotateVector(SpawnOffset);

	SpawnActor(SpawnLocation, SpawnRotator);

	if (UWorld* MyWorld = MeshComp->GetWorld())
	{
		// Don't set timer in preview world to avoid memory leak
		if (MyWorld->IsPreviewWorld())
		{
			return;
		}

		// Setup timers
		if (bAttachToOwner)
		{
			if (AttachDelay > 0.f)
			{
				MyWorld->GetTimerManager().SetTimer(DelayAttachTimerHandle, this, &UAnimNotify_SpawnActor::AttachToOwner, AttachDelay);
			}
			else
			{
				AttachToOwner();
			}
		}

		if (bAutoDetach)
		{
			if (AutoDetachDelay > 0.f)
			{
				MyWorld->GetTimerManager().SetTimer(AutoDetachTimerHandle, this, &UAnimNotify_SpawnActor::DetachFromOwner, AttachDelay + AutoDetachDelay);
			}
			else
			{
				DetachFromOwner();
			}
		}
	}
}

void UAnimNotify_SpawnActor::SpawnActor(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!IsValid(ActorClass))
	{
		UE_LOG(LogAnimation, Error, TEXT("Invalid actor class to spawn."));
		return;
	}

	if (Lifespan <= 0.f)
	{
		UE_LOG(LogAnimation, Warning, TEXT("Actor is not spawned due to negative lifespan specified."));
		return;
	}

	if (UWorld* MyWorld = OwnerComponent->GetWorld())
	{
		// Don't spawn in preview world to avoid memory leak
		if (MyWorld->IsPreviewWorld())
		{
			return;
		}

		if (AActor* NewActor = MyWorld->SpawnActor<AActor>(ActorClass, SpawnLocation, SpawnRotation))
		{
			NewActor->SetLifeSpan(Lifespan);
			SpawnedActor = NewActor;
		}
	}
}

void UAnimNotify_SpawnActor::AttachToOwner()
{
	if (!IsValid(SpawnedActor))
	{
		return;
	}

	if (OwnerComponent != nullptr)
	{
		SpawnedActor->AttachToComponent(OwnerComponent, FAttachmentTransformRules::KeepWorldTransform, SocketName);
	}
}

void UAnimNotify_SpawnActor::DetachFromOwner()
{
	if (!IsValid(SpawnedActor))
	{
		return;
	}

	SpawnedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}


