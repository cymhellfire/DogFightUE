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

	virtual void RemoveFromTarget(TSharedPtr<FAttributeBase>& OutTarget) override;

	virtual TSharedPtr<FAttributeModifierBase> GetModifier() override
	{
		return Modifier;
	}

	virtual UAttributeModifierDescObject* GetDescObject(UObject* Instigator) override;

protected:
	virtual void OnModifierCreated(TSharedPtr<FAttributeModifierBase> InModifier) override;

	virtual UAttributeModifierDescObject* GenerateDescObject(UObject* Instigator) override;

	virtual TSubclassOf<UAttributeModifierDescObject> GetModifierDescClass() override;

	virtual void InitializeDescObject(UAttributeModifierDescObject* InDesc);

protected:
	TSharedPtr<FAttributeModifierBase> Modifier;

	UPROPERTY(Transient)
	UAttributeModifierDescObject* DescObject = nullptr;
};
