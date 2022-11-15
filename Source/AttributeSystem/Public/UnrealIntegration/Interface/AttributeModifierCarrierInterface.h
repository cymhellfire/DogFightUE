#pragma once

#include "AttributeSystem/Modifier/AttributeModifierCommon.h"
#include "AttributeModifierCarrierInterface.generated.h"

class FAttributeModifierBase;
class UAttributeModifierDescObject;

UINTERFACE(NotBlueprintable)
class ATTRIBUTESYSTEM_API UAttributeModifierCarrierInterface : public UInterface
{
	GENERATED_BODY()
};

class ATTRIBUTESYSTEM_API IAttributeModifierCarrierInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual bool CreateModifier(const FModifierCreateArgument& InArgument);

	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual void RemoveFromTarget();

	virtual TSharedPtr<FAttributeModifierBase> GetModifier() = 0;

	virtual UAttributeModifierDescObject* GenerateDescObject(UObject* Instigator) = 0;

protected:
	virtual void OnModifierCreated(TSharedPtr<FAttributeModifierBase> InModifier) = 0;

	virtual TSubclassOf<UAttributeModifierDescObject> GetModifierDescClass() = 0;
};
