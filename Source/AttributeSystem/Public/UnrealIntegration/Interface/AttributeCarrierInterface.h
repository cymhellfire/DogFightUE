#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Attribute/AttributeCommon.h"
#include "UnrealIntegration/Marco/AttributeCarrierMarcos.h"
#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"
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
	 * @brief Create a new boolean type attribute for this class.
	 * @param InName			Name of new attribute.
	 * @param InitValue			Initial value of new attribute.
	 * @param Tags				Tag string of new attribute.
	 * @return Whether the creating succeed
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddBooleanAttribute(FName InName, bool InitValue = false, FString Tags = "");

	/**
	 * @brief Create a new integer type attribute for this class.
	 * @param InName			Name of new attribute.
	 * @param InitValue			Initial value of new attribute.
	 * @param Tags				Tag string of new attribute.
	 * @return Whether the creating succeed
	 */
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddIntegerAttribute(FName InName, int32 InitValue = 0, FString Tags = "");

	/**
	 * @brief Create a new float type attribute for this class.
	 * @param InName			Name of new attribute.
	 * @param InitValue			Initial value of new attribute.
	 * @param Tags				Tag string of new attribute.
	 * @return Whether the creating succeed
	 */
	virtual bool AddFloatAttribute(FName InName, float InitValue = 0.f, FString Tags = "");

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
	// ------------------------------------|==========|------------------------------------
	// ------------------------------------|Attributes|------------------------------------
	// ------------------------------------|==========|------------------------------------

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
	 * @brief Get local net role of this instance.
	 * @return Net role.
	 */
	virtual ENetRole GetNetRole() = 0;

	// ------------------------------------|==========|------------------------------------
	// ------------------------------------| Modifier |------------------------------------
	// ------------------------------------|==========|------------------------------------

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
	 * @brief Create new wrapper object for given attribute.
	 * @param InAttribute Attribute that new wrapper object created for.
	 */
	void CreateWrapperObjectForAttribute(TSharedPtr<FAttributeBase> InAttribute);

	/**
	 * @brief Notify the class a new wrapper object for boolean attribute is created.
	 * @param NewWrapper New created wrapper object.
	 */
	virtual void OnBooleanAttributeWrapperObjectCreated(UAttributeBooleanWrapperObject* NewWrapper) = 0;

	/**
	 * @brief Notify the class a new wrapper object for integer attribute is created.
	 * @param NewWrapper New created wrapper object.
	 */
	virtual void OnIntegerAttributeWrapperObjectCreated(UAttributeIntegerWrapperObject* NewWrapper) = 0;

	/**
	 * @brief Notify the class a new wrapper object for float attribute is created.
	 * @param NewWrapper New created wrapper object.
	 */
	virtual void OnFloatAttributeWrapperObjectCreated(UAttributeFloatWrapperObject* NewWrapper) = 0;

	/**
	 * @brief Function to update recorded description objects in wrapper properties.
	 * @param AppliedAttribute			Attribute instance that indicate wrapper property.
	 * @param InDescObject				Description object that to update.
	 * @param bAdd						Add/Remove description object to/from wrapper property.
	 */
	void UpdateDescObjectToProperty(TSharedPtr<FAttributeBase> AppliedAttribute, UAttributeModifierDescObject* InDescObject, bool bAdd);

	/**
	 * @brief Getter function for dynamically created wrapper struct of boolean attribute.
	 * @param InName			Name of attribute to search.
	 * @return The wrapper struct if found.
	 */
	UAttributeBooleanWrapperObject* GetBooleanAttributeWrapperByName(FName InName);

	/**
	 * @brief Getter function for dynamically created wrapper struct of integer attribute.
	 * @param InName			Name of attribute to search.
	 * @return The wrapper struct if found.
	 */
	UAttributeIntegerWrapperObject* GetIntegerAttributeWrapperByName(FName InName);

	/**
	 * @brief Getter function for dynamically created wrapper struct of float attribute.
	 * @param InName			Name of attribute to search.
	 * @return The wrapper struct if found.
	 */
	UAttributeFloatWrapperObject* GetFloatAttributeWrapperByName(FName InName);

	/**
	 * @brief Iterate through specified wrapper object map and remove invalid records.
	 * @param InDataType		Data type that to validate.
	 * @param OutInvalidKeys	Invalid keys that has been removed. (Optional)
	 */
	void ValidateWrapperObjectMap(EAttributeDataType InDataType, TArray<FName>* OutInvalidKeys = nullptr);

	/**
	 * @brief Get all recorded boolean attribute wrappers.
	 * @return List of all attribute wrappers.
	 */
	virtual TArray<UAttributeBooleanWrapperObject*> GetAllBooleanAttributeWrappers() const = 0;

	/**
	 * @brief Get all recorded integer attribute wrappers.
	 * @return List of all attribute wrappers.
	 */
	virtual TArray<UAttributeIntegerWrapperObject*> GetAllIntegerAttributeWrappers() const = 0;

	/**
	 * @brief Get all recorded float attribute wrappers.
	 * @return List of all attribute wrappers.
	 */
	virtual TArray<UAttributeFloatWrapperObject*> GetAllFloatAttributeWrappers() const = 0;

	// ------------------------------------|==========|------------------------------------
	// ------------------------------------|Networking|------------------------------------
	// ------------------------------------|==========|------------------------------------

	/**
	 * @brief Network OnRep function for boolean attribute wrapper list. Override it for customize purposes.
	 */
	UFUNCTION()
	virtual void OnRep_BooleanWrapperList();

	/**
	 * @brief Network OnRep function for integer attribute wrapper list. Override it for customize purposes.
	 */
	UFUNCTION()
	virtual void OnRep_IntegerWrapperList();

	/**
	 * @brief Network OnRep function for float attribute wrapper list. Override it for customize purposes.
	 */
	UFUNCTION()
	virtual void OnRep_FloatWrapperList();

	/**
	 * @brief Notify the class a new boolean attribute wrapper is synced from network for adding. (Client only.)
	 * @param InWrapper New wrapper added.
	 */
	virtual void Sync_OnBooleanWrapperAdded(UAttributeBooleanWrapperObject* InWrapper) {}

	/**
	 * @brief Notify the class a new integer attribute wrapper is synced from network for adding.  (Client only.)
	 * @param InWrapper New wrapper added.
	 */
	virtual void Sync_OnIntegerWrapperAdded(UAttributeIntegerWrapperObject* InWrapper) {}

	/**
	 * @brief Notify the class a new float attribute wrapper is synced from network for adding.  (Client only.)
	 * @param InWrapper New wrapper added.
	 */
	virtual void Sync_OnFloatWrapperAdded(UAttributeFloatWrapperObject* InWrapper) {}

	/**
	 * @brief Whether add missing attributes needed by new added modifier automatically.
	 */
	bool bAddMissingAttributeForModifier = false;

	// Map container that do NOT replicated. These maps are maintained by OnRep functions on client side.
	TMap<FName, TWeakObjectPtr<UAttributeBooleanWrapperObject>> BooleanWrapperMap;
	TMap<FName, TWeakObjectPtr<UAttributeIntegerWrapperObject>> IntegerWrapperMap;
	TMap<FName, TWeakObjectPtr<UAttributeFloatWrapperObject>> FloatWrapperMap;
};
