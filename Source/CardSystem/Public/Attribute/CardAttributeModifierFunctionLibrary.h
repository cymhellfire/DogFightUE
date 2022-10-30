#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Modifier/AttributeModifierCommon.h"

class UCard;
class FAttributeModifierBoolean;
class FAttributeModifierInteger;
class FAttributeModifierFloat;

class CARDSYSTEM_API FCardAttributeModifierFunctionLibrary
{
public:
	static TSharedPtr<FAttributeModifierBoolean> CreateBoolAttributeModifier(FName InName, bool InValue);

	static TSharedPtr<FAttributeModifierInteger> CreateIntegerAttributeModifier(FName InName, int32 InValue, EModifierOperatorType OpType, FString ApplyRule);

	static TSharedPtr<FAttributeModifierFloat> CreateFloatAttributeModifier(FName InName, float InValue, EModifierOperatorType OpType);
};
