#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Attribute/AttributeCommon.h"
#include "UnrealIntegration/Marco/AttributeCarrierMarcos.h"
#include "UnrealIntegration/DataWrapper/AttributeWrapper.h"
#include "AttributeCarrierInterface.generated.h"

class FAttributeBase;
class FAttributeModifierBase;
class IAttributeModifierCarrierInterface;
class UAttributeModifierDescObject;

UINTERFACE(NotBlueprintable)
class ATTRIBUTESYSTEM_API UAttributeCarrierInterface : public UInterface
{
	GENERATED_BODY()
};

class ATTRIBUTESYSTEM_API IAttributeCarrierInterface
{
	GENERATED_BODY()
public:
	/**
	 * @brief Create a new attribute for this class.
	 * @param InArgument		Attribute creating arguments.
	 * @return Whether the creating succeed
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddAttribute(const FAttributeCreateArgument& InArgument);

	/**
	 * @brief Get value from a boolean attribute with given name.
	 * @param InName			Name of expected attribute.
	 * @param OutValue			The value obtained.
	 * @return Is the attribute found?
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeBoolValue(FName InName, bool& OutValue);

	/**
	 * @brief Get value from a integer attribute with given name.
	 * @param InName			Name of expected attribute.
	 * @param OutValue			The value obtained.
	 * @return Is the attribute found?
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeIntegerValue(FName InName, int32& OutValue);

	/**
	 * @brief Get value from a float attribute with given name.
	 * @param InName			Name of expected attribute.
	 * @param OutValue			The value obtained.
	 * @return Is the attribute found?
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeFloatValue(FName InName, float& OutValue);

	/**
	 * @brief Remove attribute from class by name.
	 * @param InName			Name of attribute to remove.
	 * @return Is the removal succeed?
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveAttribute(FName InName) = 0;

	/**
	 * @brief Apply a modifier object to this instance.
	 * @param InModifierObject		Modifier object to apply.
	 * @return Is the modifier applied successfully?
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject);

	/**
	 * @brief Remove a applied modifier object from this instance.
	 * @param InModifierObject		Modifier to remove.
	 * @return Is the modifier removed successfully?
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject);

	/**
	 * Replication function for all modifier description objects.
	 */
	virtual bool ReplicateModifierDescObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) = 0;

protected:
	/**
	 * @brief Notify the class a new attribute is added.
	 * @param InAttribute			New added attribute.
	 * @return Whether to record new attribute successfully
	 */
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) = 0;

	/**
	 * @brief Get attribute with specified name.
	 * @param InName			Name of expected attribute.
	 * @return The found attribute or nullptr
	 */
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) = 0;

	/**
	 * @brief Get all available attributes as array.
	 * @return Array of all added attributes.
	 */
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() = 0;

	/**
	 * @brief Get all attributes have given data type.
	 * @param InDataType		Data type to filter attributes.
	 * @return Attribute list
	 */
	TArray<TSharedPtr<FAttributeBase>> GetAttributesByDataType(EAttributeDataType InDataType);
	/**
	 * @brief Get all attributes meet tags requirements.
	 * @param InTags			Tags array to filter attributes.
	 * @param bMatchAll			Whether to filter attribute with all tags.
	 * @return Attribute list
	 */
	TArray<TSharedPtr<FAttributeBase>> GetAttributesByTags(const TArray<FName>& InTags, bool bMatchAll);

	/**
	 * Get UObject that used to hold all sub object. Also used as the replication parent.
	 */
	virtual UObject* GetSubobjectCarrier() = 0;

	/**
	 * Get UObject that represent this interface instance.
	 */
	virtual UObject* ThisAsObject() = 0;

	/**
	 * Find a suitable attribute and apply passed in modifier to it.
	 * @param InModifier			Modifier to apply.
	 * @param OutAttribute			The attribute applied new modifier.
	 * @return Is the modifier applied successfully
	 */
	bool AddAttributeModifier(TSharedPtr<FAttributeModifierBase> InModifier, TSharedPtr<FAttributeBase>& OutAttribute);
	/**
	 * @brief Check if given modifier is applied.
	 * @param InModifier			Modifier to check.
	 * @return Whether the modifier is applied or not.
	 */
	bool IsModifierObjectApplied(IAttributeModifierCarrierInterface* InModifier) const;

	/**
	 * @brief Notify the class a new modifier is applied in interface form.
	 * @param InModifierInterface		Modifier is applied
	 */
	virtual void OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface) = 0;
	/**
	 * @brief Notify the class a new modifier is applied in UObject form.
	 * @param InModifierObject			Modifier is applied
	 */
	virtual void OnModifierObjectAdded(UObject* InModifierObject) = 0;
	/**
	 * @brief Notify the class a new modifier description object is added.
	 * @param InModifierObject			Modifier owning the description object.
	 * @param InDescObject				New description object.
	 */
	virtual void OnModifierDescObjectAdded(UObject* InModifierObject, UAttributeModifierDescObject* InDescObject) = 0;
	/**
	 * @brief Notify the class a modifier is removed in interface form.
	 * @param InModifierInterface			Modifier is removed.
	 */
	virtual void OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierInterface) = 0;
	/**
	 * @brief Notify the class a modifier is removed in UObject form.
	 * @param InModifierObject				Modifier is removed.
	 */
	virtual void OnModifierObjectRemoved(UObject* InModifierObject) = 0;
	/**
	 * @breif Get all applied modifier objects.
	 * @return Array of all applied modifier objects.
	 */
	virtual TArray<IAttributeModifierCarrierInterface*> GetAllModifierObjects() const = 0;

	/**
	 * @brief Function to update recorded description objects in wrapper properties. UE reflection system version.
	 * @param AppliedAttribute			Attribute instance that indicate wrapper property.
	 * @param InDescObject				Description object that to update.
	 * @param bAdd						Add/Remove description object to/from wrapper property.
	 */
	void UpdateDescObjectToProperty_Reflection(TSharedPtr<FAttributeBase> AppliedAttribute, UAttributeModifierDescObject* InDescObject, bool bAdd);

	/**
	 * @brief Function to update recorded description objects in wrapper properties. Use custom getter function version.
	 * @param AppliedAttribute			Attribute instance that indicate wrapper property.
	 * @param InDescObject				Description object that to update.
	 * @param bAdd						Add/Remove description object to/from wrapper property.
	 */
	void UpdateDescObjectToProperty_Dynamic(TSharedPtr<FAttributeBase> AppliedAttribute, UAttributeModifierDescObject* InDescObject, bool bAdd);

	/**
	 * @brief When add description objects, use UE reflection system to search attribute wrapper member declared in code
	 * is default behavior. If the wrapper struct is created dynamically in runtime, you can use custom getter function
	 * return correct result.
	 * @return Whether use dynamic wrapper getter functions.
	 */
	virtual bool UseCustomAttributeWrapperGetter() const { return false; }

	/**
	 * @brief Getter function for dynamically created wrapper struct of boolean attribute.
	 * @param InName			Name of attribute to search.
	 * @return The wrapper struct if found.
	 */
	virtual TSharedPtr<FAttributeBooleanWrapper> GetBooleanAttributeWrapperByName(FName InName) { return nullptr; }

	/**
	 * @brief Getter function for dynamically created wrapper struct of integer attribute.
	 * @param InName			Name of attribute to search.
	 * @return The wrapper struct if found.
	 */
	virtual TSharedPtr<FAttributeIntegerWrapper> GetIntegerAttributeWrapperByName(FName InName) { return nullptr; }

	/**
	 * @brief Getter function for dynamically created wrapper struct of float attribute.
	 * @param InName			Name of attribute to search.
	 * @return The wrapper struct if found.
	 */
	virtual TSharedPtr<FAttributeFloatWrapper> GetFloatAttributeWrapperByName(FName InName) { return nullptr; }
};
