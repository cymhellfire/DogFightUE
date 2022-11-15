#pragma once

#include "AttributeWrapper.generated.h"

USTRUCT(BlueprintType)
struct FAttributeBooleanWrapper
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeBooleanWrapper")
	bool BaseValue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeBooleanWrapper")
	bool Value;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeBooleanWrapper", Transient)
	TArray<UObject*> AppliedModifierDesc;
};

USTRUCT(BlueprintType)
struct FAttributeIntegerWrapper
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeIntegerWrapper")
	int32 BaseValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeIntegerWrapper")
	int32 Value;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeIntegerWrapper", Transient)
	TArray<UObject*> AppliedModifierDesc;
};

USTRUCT(BlueprintType)
struct FAttributeFloatWrapper
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeFloatWrapper")
	float BaseValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeFloatWrapper")
	float Value;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeFloatWrapper", Transient)
	TArray<UObject*> AppliedModifierDesc;
};
