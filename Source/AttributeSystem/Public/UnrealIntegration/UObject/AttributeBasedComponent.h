#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"
#include "AttributeBasedComponent.generated.h"

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeBasedComponent : public UActorComponent, public IAttributeCarrierInterface
{
	GENERATED_BODY()
public:
	UAttributeBasedComponent(const FObjectInitializer& ObjectInitializer);

	// -------- Attribute Carrier Interface --------
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddAttribute(const FAttributeCreateArgument& InArgument) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeBoolValue(FName InName, bool& OutValue) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeIntegerValue(FName InName, int32& OutValue) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool GetAttributeFloatValue(FName InName, float& OutValue) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveAttribute(FName InName) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool AddModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject) override;
protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) override;
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) override;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() override;

	virtual void OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface) override;
	virtual void OnModifierObjectAdded(UObject* InModifierObject) override;
	virtual void OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierInterface) override;
	virtual void OnModifierObjectRemoved(UObject* InModifierObject) override;
	virtual TArray<IAttributeModifierCarrierInterface*> GetAllModifierObjects() const override;
	// -------- Attribute Carrier Interface --------

protected:
	TMap<FName, TSharedPtr<FAttributeBase>> AttributeMap;

	TArray<IAttributeModifierCarrierInterface*> ModifierList;

	// Reference holder for modifiers
	UPROPERTY(Transient)
	TArray<UObject*> ModifierObjectList;
};
