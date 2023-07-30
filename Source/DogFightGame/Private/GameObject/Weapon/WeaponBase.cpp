// Dog Fight Game Code By CYM.

#include "GameObject/Weapon/WeaponBase.h"
#include "DataAsset/WeaponActionDataAsset.h"
#include "GameObject/Weapon/WeaponActionBase.h"

void UWeaponBase::AddWeaponAction(UWeaponActionDataAsset* InActionData)
{
	if (!IsValid(InActionData))
	{
		return;
	}

	// Create new weapon action
	auto NewAction = NewObject<UWeaponActionBase>(this);
	if (IsValid(NewAction))
	{
		NewAction->InitActionData(InActionData, nullptr);
		WeaponActions.AddUnique(NewAction);
	}
}
