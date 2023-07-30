#pragma once

#include "UnrealIntegration/Interface/AttributeModifierCarrierInterface.h"
#include "AttributeModifierBasedObject.generated.h"

class FAttributeModifierBase;
class UAttributeWrapperObjectBase;

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeModifierBasedObject : public UObject, public IAttributeModifierCarrierInterface
{
	GENERATED_BODY()
public:
	virtual TSharedPtr<FAttributeModifierBase> GetModifier() override
	{
		return Modifier;
	}

	virtual UAttributeModifierDescObject* GetDescObject(UObject* Instigator) override;

	/**
	 * Get the wrapper object of attribute this modifier applied to.
	 * @return The wrapper object of modified attribute.
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeModifierBasedObject")
	virtual UAttributeWrapperObjectBase* GetModifiedAttributeObject() const;

	virtual bool SetModifierBoolFactor(bool InFactor) override;
	virtual bool SetModifierIntegerFactor(int32 InFactor) override;
	virtual bool SetModifierFloatFactor(float InFactor) override;

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
