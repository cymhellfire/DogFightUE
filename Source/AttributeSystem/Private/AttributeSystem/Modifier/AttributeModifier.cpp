#include "AttributeSystem/Modifier/AttributeModifier.h"

#define ATTR_LOC_ST_PATH		"/Game/DogFightGame/DataTable/Localization/ST_Attribute"
#define VALUE_INC_TAG			TEXT("<Value.Increase>")
#define VALUE_DEC_TAG			TEXT("<Value.Decrease>")
#define TAG_END					TEXT("</>")

FString FAttributeModifierBoolean::GetEffectString() const
{
	return FString::Printf(TEXT("Override to %s"), ModifyFactor ? TEXT("true") : TEXT("false"));
}

FText FAttributeModifierBoolean::GetLocalizedEffectString()
{
	FText FormatText = FText::FromStringTable(ATTR_LOC_ST_PATH, "Effect_Set");
	FString ValueKey = ModifiedValue ? "Value_True" : "Value_False";
	FFormatOrderedArguments Arguments;
	Arguments.Add(FFormatArgumentValue(FText::FromStringTable(ATTR_LOC_ST_PATH, ValueKey)));

	return FText::Format(FormatText, Arguments);
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

FText FAttributeModifierInteger::GetLocalizedEffectString()
{
	FText FormatText;
	FFormatOrderedArguments Arguments;
	if (OperatorType == EModifierOperatorType::MOT_Equal)
	{
		FormatText = FText::FromStringTable(ATTR_LOC_ST_PATH, "Effect_Set");
		Arguments.Add(FFormatArgumentValue(ModifyFactor));
	}
	else
	{
		auto Delta = GetModifiedValue() - GetOriginValue();
		const bool bIncreased = Delta >= 0;
		FormatText = FText::FromStringTable(ATTR_LOC_ST_PATH, bIncreased ? "Effect_Increase" : "Effect_Decrease");
		Arguments.Add(FFormatArgumentValue(FText::FromString(
			FString::Printf(TEXT("%s%.0f%%%s"), (bIncreased ? VALUE_INC_TAG : VALUE_DEC_TAG),
				FMath::Abs((float)Delta / GetOriginValue() * 100.f), TAG_END))));
	}

	return FText::Format(FormatText, Arguments);
}

FString FAttributeModifierFloat::GetEffectString() const
{
	return FString::Printf(TEXT("%s %.3f"), *ConvertOperatorTypeToString(OperatorType), ModifyFactor);
}

FText FAttributeModifierFloat::GetLocalizedEffectString()
{
	FText FormatText;
	FFormatOrderedArguments Arguments;
	if (OperatorType == EModifierOperatorType::MOT_Equal)
	{
		FormatText = FText::FromStringTable(ATTR_LOC_ST_PATH, "Effect_Set");
		Arguments.Add(FFormatArgumentValue(ModifyFactor));
	}
	else
	{
		auto Delta = GetModifiedValue() - GetOriginValue();
		const bool bIncreased = Delta >= 0;
		FormatText = FText::FromStringTable(ATTR_LOC_ST_PATH, bIncreased ? "Effect_Increase" : "Effect_Decrease");
		Arguments.Add(FFormatArgumentValue(FText::FromString(
			FString::Printf(TEXT("%s%.0f%%%s"), (bIncreased ? VALUE_INC_TAG : VALUE_DEC_TAG),
				FMath::Abs((float)Delta / GetOriginValue() * 100.f), TAG_END))));
	}

	return FText::Format(FormatText, Arguments);
}
