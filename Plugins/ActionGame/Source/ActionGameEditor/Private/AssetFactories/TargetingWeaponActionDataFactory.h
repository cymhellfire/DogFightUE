// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "TargetingWeaponActionDataFactory.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAMEEDITOR_API UTargetingWeaponActionDataFactory : public UFactory
{
	GENERATED_BODY()
public:
	UTargetingWeaponActionDataFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
