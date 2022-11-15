#pragma once

#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"
#include "TestAttributeModifier.generated.h"

UCLASS()
class DOGFIGHT_API UTestAttributeModifier : public UAttributeModifierBasedObject
{
	GENERATED_BODY()
public:
	virtual void PostInitProperties() override;

protected:
	virtual void InitializeDescObject(UAttributeModifierDescObject* InDesc) override;
};
