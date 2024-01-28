// Dog Fight Game Code By CYM.

#include "TargetingWeaponActionDataFactory.h"
#include "DataAsset/TargetingWeaponActionDataAsset.h"

UTargetingWeaponActionDataFactory::UTargetingWeaponActionDataFactory()
{
	SupportedClass = UTargetingWeaponActionDataAsset::StaticClass();
	bCreateNew = true;
}

UObject* UTargetingWeaponActionDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UTargetingWeaponActionDataAsset>(InParent, InClass, InName, Flags, Context);
}
