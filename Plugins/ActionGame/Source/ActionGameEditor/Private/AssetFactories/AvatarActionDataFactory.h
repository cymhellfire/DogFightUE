// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AvatarActionDataFactory.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAMEEDITOR_API UAvatarActionDataFactory : public UFactory
{
	GENERATED_BODY()
public:
	UAvatarActionDataFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
