#pragma once

#include "UnrealAttribute.generated.h"

USTRUCT(BlueprintType)
struct ATTRIBUTESYSTEM_API FNetAttributeBoolean
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="AttributeBoolean")
	bool Value;
};

USTRUCT(BlueprintType)
struct ATTRIBUTESYSTEM_API FNetAttributeInteger
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="AttributeInteger")
	int32 Value;

	UPROPERTY(BlueprintReadWrite, Category="AttributeInteger")
	int32 BaseValue;
};

USTRUCT(BlueprintType)
struct ATTRIBUTESYSTEM_API FNetAttributeFloat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="AttribtueFloat")
	float Value;

	UPROPERTY(BlueprintReadWrite, Category="AttributeFloat")
	float BaseValue;
};
