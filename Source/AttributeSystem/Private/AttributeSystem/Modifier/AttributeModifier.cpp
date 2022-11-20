#include "AttributeSystem/Modifier/AttributeModifier.h"

FString FAttributeModifierBoolean::GetEffectString() const
{
	return FString::Printf(TEXT("Override to %s"), ModifyFactor ? TEXT("true") : TEXT("false"));
}

void FAttributeModifierBoolean::UpdateModifiedValue()
{
	// Directly override value
	ModifiedValue = ModifyFactor;

	bDirty = false;
}

FString ConvertOperatorTypeToString(EModifierOperatorType InType)
{
	FString OpText;
	switch(InType)
	{
	case EModifierOperatorType::MOT_Equal:
		OpText = "=";
		break;
	case EModifierOperatorType::MOT_Addition:
		OpText = "+";
		break;
	case EModifierOperatorType::MOT_Subtract:
		OpText = "-";
		break;
	case EModifierOperatorType::MOT_Multiply:
		OpText = "x";
		break;
	case EModifierOperatorType::MOT_Divide:
		OpText = "/";
		break;
	default: ;
	}

	return OpText;
}

FString FAttributeModifierInteger::GetEffectString() const
{
	return FString::Printf(TEXT("%s %d"), *ConvertOperatorTypeToString(OperatorType), ModifyFactor);
}

FString FAttributeModifierFloat::GetEffectString() const
{
	return FString::Printf(TEXT("%s %.3f"), *ConvertOperatorTypeToString(OperatorType), ModifyFactor);
}
