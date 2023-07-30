// Dog Fight Game Code By CYM.

#include "GameObject/Weapon/WeaponActionBase.h"
#include "DataAsset/WeaponActionDataAsset.h"
#include "Interface/ActionCharacterInterface/ActionCharacterInterface.h"

UWeaponActionBase::UWeaponActionBase()
{
	Performer = nullptr;
}

void UWeaponActionBase::InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* Owner)
{
	if (!IsValid(InData))
	{
		return;
	}

	ActionName = InData->Name;
	ActionDescription = InData->Description;
	bNeedTarget = InData->bNeedTarget;
	ActionRange = InData->Range;
	ActionMontage = InData->AnimMontage.IsValid() ? InData->AnimMontage.Get() : InData->AnimMontage.LoadSynchronous();

	Performer = Owner;
}

void UWeaponActionBase::SetActionTarget(const FWeaponActionTarget& InTarget)
{
	// Skip if target is unnecessary
	if (!bNeedTarget)
	{
		return;
	}

	ActionTarget = InTarget;
}

void UWeaponActionBase::Execute()
{
	if (!Performer)
		return;

	if (CheckDistance())
	{
		
	}
}

bool UWeaponActionBase::CheckDistance()
{
	return false;
}
