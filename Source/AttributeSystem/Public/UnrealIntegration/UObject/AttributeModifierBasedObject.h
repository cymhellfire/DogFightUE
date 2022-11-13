#pragma once

#include "UnrealIntegration/Interface/AttributeModifierCarrierInterface.h"
#include "AttributeModifierBasedObject.generated.h"

class FAttributeModifierBase;

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeModifierBasedObject : public UObject, public IAttributeModifierCarrierInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual bool CreateModifier(const FModifierCreateArgument& InArgument) override;

	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual void RemoveFromTarget() override;

	virtual TSharedPtr<FAttributeModifierBase> GetModifier() override
	{
		return Modifier;
	}

protected:
	virtual void OnModifierCreated(TSharedPtr<FAttributeModifierBase> InModifier) override;

protected:
	TSharedPtr<FAttributeModifierBase> Modifier;
};
