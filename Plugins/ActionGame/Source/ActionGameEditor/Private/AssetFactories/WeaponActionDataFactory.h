// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "WeaponActionDataFactory.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAMEEDITOR_API UWeaponActionDataFactory : public UFactory
{
	GENERATED_BODY()
public:
	UWeaponActionDataFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
