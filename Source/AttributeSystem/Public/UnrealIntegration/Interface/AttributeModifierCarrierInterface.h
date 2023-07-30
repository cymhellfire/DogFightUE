#pragma once

#include "AttributeSystem/Modifier/AttributeModifierCommon.h"
#include "AttributeModifierCarrierInterface.generated.h"

class FAttributeBase;
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
	/**
	 * @brief Create the inner struct for this modifier.
	 * @param InArgument		Creating argument for new modifier.
	 * @return Is the creating successfully?
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual bool CreateModifier(const FModifierCreateArgument& InArgument);

	/**
	 * @brief Set the factor value of this bool modifier.
	 * @param InFactor			New factor value.
	 * @return Whether the factor is updated succeed.
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual bool SetModifierBoolFactor(bool InFactor) = 0;

	/**
	 * @brief Set the factor value of this integer modifier.
	 * @param InFactor			New factor value.
	 * @return Whether the factor is updated succeed.
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual bool SetModifierIntegerFactor(int32 InFactor) = 0;

	/**
	 * @brief Set the factor value of this float modifier.
	 * @param InFactor			New factor value.
	 * @return Whether the factor is updated succeed.
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeModifierCarrier")
	virtual bool SetModifierFloatFactor(float InFactor) = 0;

	/**
	 * @brief Remove this modifier from applied target.
	 * @param OutTarget			Applied target of this modifier.
	 */
	void RemoveFromTarget(TSharedPtr<FAttributeBase>& OutTarget);

	/**
	 * @brief Get the inner modifier struct.
	 * @return The inner modifier struct.
	 */
	virtual TSharedPtr<FAttributeModifierBase> GetModifier() = 0;

	/**
	 * @brief Get or generated the description object for this modifier instance.
	 * @param Instigator		The UObject instance act as description object's owner.
	 */
	virtual UAttributeModifierDescObject* GetDescObject(UObject* Instigator) = 0;

protected:
	/**
	 * @brief Notify the modifier class inner struct is created.
	 * @param InModifier		The inner struct instance.
	 */
	virtual void OnModifierCreated(TSharedPtr<FAttributeModifierBase> InModifier) = 0;

	/**
	 * @brief Generated a new description object.
	 * @param Instigator		The UObject instance act as description object's owner.
	 * @return New description object.
	 */
	virtual UAttributeModifierDescObject* GenerateDescObject(UObject* Instigator) = 0;

	/**
	 * @brief Get the base class of description object.
	 * @return The class to spawn as description object.
	 */
	virtual TSubclassOf<UAttributeModifierDescObject> GetModifierDescClass() = 0;
};
