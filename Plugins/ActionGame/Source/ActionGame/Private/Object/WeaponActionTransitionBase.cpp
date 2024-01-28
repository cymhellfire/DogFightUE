// Dog Fight Game Code By CYM.

#include "Object//WeaponActionTransitionBase.h"

#include "Object/WeaponActionBase.h"
#include "Object/WeaponBase.h"

void UWeaponActionTransitionBase::InitTransition(UWeaponBase* InOwner, EWeaponActionInput InInput, UWeaponActionBase* InAction)
{
	OwnerWeapon = InOwner;
	KeyInput = InInput;
	TargetAction = InAction;
}

void UWeaponActionTransitionBase::DoTransition(const FWeaponActionTarget& InTarget)
{
	if (!IsValid(OwnerWeapon))
	{
		return;
	}

	OwnerWeapon->PerformAction(TargetAction, InTarget);
}
