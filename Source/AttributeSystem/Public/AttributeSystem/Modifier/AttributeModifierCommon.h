#pragma once

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
	EModifierOperatorType OperatorType;
};
