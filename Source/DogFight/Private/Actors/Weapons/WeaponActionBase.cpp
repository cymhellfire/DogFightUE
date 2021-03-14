// Dog Fight Game Code By CYM.


#include "Actors/Weapons/WeaponActionBase.h"
#include "Actors/Weapons/WeaponBase.h"

void UWeaponActionBase::EnterAction()
{
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

void UWeaponActionBase::OnActionMontageFinished()
{
	if (ActionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionTimerHandle);
	}

	// Trigger callback
	OnWeaponActionFinished.Broadcast();
}
