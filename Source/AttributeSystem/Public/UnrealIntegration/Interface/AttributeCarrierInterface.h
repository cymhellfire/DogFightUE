#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Attribute/AttributeCommon.h"
#include "AttributeCarrierInterface.generated.h"

class FAttributeBase;

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
	virtual bool GetAttributeBoolValue(FName InName);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual int32 GetAttributeIntegerValue(FName InName);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual float GetAttributeFloatValue(FName InName);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveAttribute(FName InName) = 0;

protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) = 0;

	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) = 0;

	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() = 0;

	virtual TArray<TSharedPtr<FAttributeBase>> GetAttributesByDataType(EAttributeDataType InDataType);
	virtual TArray<TSharedPtr<FAttributeBase>> GetAttributesByTags(const TArray<FName>& InTags, bool bMatchAll);
};
