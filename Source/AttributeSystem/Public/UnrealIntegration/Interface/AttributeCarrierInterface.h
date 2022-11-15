#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Attribute/AttributeCommon.h"
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
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddAttribute(const FAttributeCreateArgument& InArgument);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeBoolValue(FName InName, bool& OutValue);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeIntegerValue(FName InName, int32& OutValue);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeFloatValue(FName InName, float& OutValue);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveAttribute(FName InName) = 0;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject);

	/**
	 * Replication function for all modifier description objects.
	 */
	virtual bool ReplicateModifierDescObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) = 0;

protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) = 0;
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) = 0;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() = 0;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAttributesByDataType(EAttributeDataType InDataType);
	virtual TArray<TSharedPtr<FAttributeBase>> GetAttributesByTags(const TArray<FName>& InTags, bool bMatchAll);

	/**
	 * Get UObject that used to hold all sub object. Also used as the replication parent.
	 */
	virtual UObject* GetSubobjectCarrier() = 0;

	virtual UObject* ThisAsObject() = 0;

	virtual bool AddAttributeModifier(TSharedPtr<FAttributeModifierBase> InModifier, TSharedPtr<FAttributeBase>& OutAttribute);
	virtual void OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface) = 0;
	virtual void OnModifierObjectAdded(UObject* InModifierObject) = 0;
	virtual void OnModifierDescObjectAdded(UObject* InModifierObject, UAttributeModifierDescObject* InDescObject) = 0;
	virtual void OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierObject) = 0;
	virtual void OnModifierObjectRemoved(UObject* InModifierObject) = 0;
	virtual TArray<IAttributeModifierCarrierInterface*> GetAllModifierObjects() const = 0;
	virtual bool IsModifierObjectApplied(IAttributeModifierCarrierInterface* InModifier) const;

	/**
	 * @brief Function to update recorded description objects in wrapper properties.
	 * @param AppliedAttribute			Attribute instance that indicate wrapper property.
	 * @param InDescObject				Description object that to update.
	 * @param bAdd						Add/Remove description object to/from wrapper property.
	 */
	void UpdateDescObjectToProperty(TSharedPtr<FAttributeBase> AppliedAttribute, UAttributeModifierDescObject* InDescObject, bool bAdd);
};
