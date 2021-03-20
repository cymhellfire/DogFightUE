// Dog Fight Game Code By CYM.


#include "Actors/Weapons/WeaponActionBase.h"

#include "Actors/Interfaces/WeaponCarrierInterface.h"
#include "Actors/Weapons/WeaponBase.h"

UWeaponActionBase::UWeaponActionBase()
{
	ActionDistance = 50.f;
}

void UWeaponActionBase::EnterAction()
{
	// Check distance from target
	AActor* WeaponTarget = nullptr;
	ACharacter* OwnerCharacter = OwnerWeapon->GetWeaponOwnerCharacter();
	IWeaponCarrierInterface* WeaponCarrier = Cast<IWeaponCarrierInterface>(OwnerCharacter);
	if (WeaponCarrier)
	{
		WeaponTarget = WeaponCarrier->GetWeaponTargetActor();

		const float Distance = WeaponTarget ? (WeaponTarget->GetActorLocation() - OwnerCharacter->GetActorLocation()).Size() : 0.f;
		// Close to target if necessary
		if (Distance > ActionDistance)
		{
			WeaponCarrier->GetCarrierReachActionDistanceEvent().AddDynamic(this, &UWeaponActionBase::OnReachedActionDistance);
			WeaponCarrier->SetActionDistance(ActionDistance);
			WeaponCarrier->MoveToActionDistance();
		}
		else
		{
			OnReachedActionDistance(nullptr);
		}
	}
	else
	{
		UE_LOG(LogDogFight, Warning, TEXT("[WeaponAction] No valid target actor specified."));

		OnReachedActionDistance(nullptr);
	}
}

FName UWeaponActionBase::GetNextActionWithInput(EWeaponActionInput NewInput)
{
	for (FWeaponActionTransition Transition : ActionTransitions)
	{
		if (Transition.TransitionInput == NewInput)
		{
			return Transition.TargetActionName;
		}
	}

	return NAME_None;
}


void UWeaponActionBase::SetOwnerWeapon(UWeaponBase* NewWeapon)
{
	if (NewWeapon == nullptr)
		return;

	OwnerWeapon = NewWeapon;
}

void UWeaponActionBase::OnReachedActionDistance(AActor* Carrier)
{
	// Unregister callback
	if (IsValid(Carrier))
	{
		if (IWeaponCarrierInterface* WeaponCarrier = Cast<IWeaponCarrierInterface>(Carrier))
		{
			WeaponCarrier->GetCarrierReachActionDistanceEvent().RemoveDynamic(this, &UWeaponActionBase::OnReachedActionDistance);
		}
	}

	if (IsValid(ActionMontage))
	{
		ACharacter* OwnerCharacter = OwnerWeapon->GetWeaponOwnerCharacter();
		USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
		if (MeshComponent != nullptr)
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				const float ActionDuration = AnimInstance->Montage_Play(ActionMontage);
				GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, this, &UWeaponActionBase::OnActionMontageFinished, ActionDuration);
			}
		}
	}
	else
	{
		// If no valid montage, trigger callback directly
		OnActionMontageFinished();
	}
}

void UWeaponActionBase::OnActionMontageFinished()
{
	if (ActionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionTimerHandle);
	}

	// Trigger callback
	OnWeaponActionFinished.Broadcast();
}
