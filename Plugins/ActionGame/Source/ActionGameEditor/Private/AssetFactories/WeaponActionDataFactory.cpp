// Dog Fight Game Code By CYM.

#include "WeaponActionDataFactory.h"
#include "DataAsset/WeaponActionDataAsset.h"

UWeaponActionDataFactory::UWeaponActionDataFactory()
{
	SupportedClass = UWeaponActionDataAsset::StaticClass();
	bCreateNew = true;
}

UObject* UWeaponActionDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UWeaponActionDataAsset>(InParent, InClass, InName, Flags, Context);
}
