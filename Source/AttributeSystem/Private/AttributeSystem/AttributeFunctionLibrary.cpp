#include "AttributeSystem/AttributeFunctionLibrary.h"

#include "AttributeSystem/ApplyRule/ApplyRuleParser.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Modifier/AttributeModifier.h"

TSharedPtr<FAttributeBase> FAttributeFunctionLibrary::CreateAttribute(const FAttributeCreateArgument& InArgument)
{
	switch (InArgument.DataType)
	{
	case ADT_Boolean:
		return MakeShareable(new FAttributeBoolean(InArgument));
	case ADT_Integer:
		return MakeShareable(new FAttributeInteger(InArgument));
	case ADT_Float:
		return MakeShareable(new FAttributeFloat(InArgument));
	case ADT_None:
	default:
		return nullptr;
	}
}

TSharedPtr<FAttributeModifierBase> FAttributeFunctionLibrary::CreateAttributeModifier(const FModifierCreateArgument& InArgument)
{
	TSharedPtr<FAttributeModifierBase> Result;
	switch (InArgument.DataType)
	{
	case ADT_Boolean:
		Result = MakeShareable(new FAttributeModifierBoolean(InArgument));
		break;
	case ADT_Integer:
		Result = MakeShareable(new FAttributeModifierInteger(InArgument));
		break;
	case ADT_Float:
		Result = MakeShareable(new FAttributeModifierFloat(InArgument));
		break;
	case ADT_None:
	default:
		return nullptr;
	}

	// Create apply rules
	TSharedPtr<FApplyRuleBase> ApplyRule;
	if (!InArgument.ApplyRuleString.IsEmpty())
	{
		ApplyRule = FApplyRuleParser::ParseApplyRuleFromString(InArgument.ApplyRuleString);
	}
	// Set apply rule
	if (ApplyRule != nullptr && ApplyRule.IsValid())
	{
		Result->SetApplyRule(ApplyRule);
	}

	return Result;
}
