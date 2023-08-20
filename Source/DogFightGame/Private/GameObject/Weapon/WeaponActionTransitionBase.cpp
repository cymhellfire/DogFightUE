// Dog Fight Game Code By CYM.

#include "GameObject/Weapon/WeaponActionTransitionBase.h"

#include "GameObject/Weapon/WeaponActionBase.h"
#include "GameObject/Weapon/WeaponBase.h"

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
