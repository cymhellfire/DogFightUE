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
	virtual bool GetAttributeBoolValue(FName InName) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual int32 GetAttributeIntegerValue(FName InName) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual float GetAttributeFloatValue(FName InName) override;

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveAttribute(FName InName) override;
protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) override;
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) override;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() override;
	// -------- Attribute Carrier Interface --------

protected:
	TMap<FName, TSharedPtr<FAttributeBase>> AttributeMap;
};
