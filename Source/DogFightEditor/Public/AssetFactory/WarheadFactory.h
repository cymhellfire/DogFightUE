#pragma once

#include "Factories/Factory.h"
#include "WarheadFactory.generated.h"

class UWarheadBase;

UCLASS()
class DOGFIGHTEDITOR_API UWarheadFactory : public UFactory
{
	GENERATED_BODY()

public:
	UWarheadFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
