#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"
#include "AttributeBasedObject.generated.h"

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeBasedObject : public UObject, public IAttributeCarrierInterface
{
	GENERATED_BODY()
public:

	// -------- Attribute Carrier Interface --------'
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
protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) override;
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) override;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() override;
	// -------- Attribute Carrier Interface --------

protected:
	TMap<FName, TSharedPtr<FAttributeBase>> AttributeMap;
};
