#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Attribute/AttributeCommon.h"
#include "AttributeCarrierInterface.generated.h"

class FAttributeBase;
class FAttributeModifierBase;
class IAttributeModifierCarrierInterface;

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

protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) = 0;
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) = 0;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() = 0;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAttributesByDataType(EAttributeDataType InDataType);
	virtual TArray<TSharedPtr<FAttributeBase>> GetAttributesByTags(const TArray<FName>& InTags, bool bMatchAll);

	virtual bool AddAttributeModifier(TSharedPtr<FAttributeModifierBase> InModifier);
	virtual void OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface) = 0;
	virtual void OnModifierObjectAdded(UObject* InModifierObject) = 0;
	virtual void OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierObject) = 0;
	virtual void OnModifierObjectRemoved(UObject* InModifierObject) = 0;
	virtual TArray<IAttributeModifierCarrierInterface*> GetAllModifierObjects() const = 0;
	virtual bool IsModifierObjectApplied(IAttributeModifierCarrierInterface* InModifier) const;
};
