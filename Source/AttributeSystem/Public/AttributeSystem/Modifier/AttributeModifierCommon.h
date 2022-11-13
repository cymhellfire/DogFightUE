#pragma once

#include "AttributeSystem/AttributeSystemCommon.h"
#include "AttributeModifierCommon.generated.h"

UENUM()
enum class EModifierOperatorType
{
	MOT_Equal,
	MOT_Addition,
	MOT_Subtract,
	MOT_Multiply,
	MOT_Divide,
};

USTRUCT(BlueprintType)
struct ATTRIBUTESYSTEM_API FModifierCreateArgument
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifierArgument")
	TEnumAsByte<EAttributeDataType> DataType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifierArgument")
	EModifierOperatorType OperatorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifierArgument")
	FString ApplyRuleString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifierArgument")
	bool InitBooleanValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifierArgument")
	int32 InitIntegerValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifierArgument")
	float InitFloatValue = 0.f;
};
