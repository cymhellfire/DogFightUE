#include "Attribute/CardAttributeModifierFunctionLibrary.h"
#include "AttributeSystem/Modifier/AttributeModifier.h"
#include "AttributeSystem/AttributeFunctionLibrary.h"

TSharedPtr<FAttributeModifierBoolean> FCardAttributeModifierFunctionLibrary::CreateBoolAttributeModifier(
	FName InName, bool InValue)
{
	FModifierCreateArgument CreateArgument;
	CreateArgument.DataType = ADT_Boolean;

	auto NewModifier = FAttributeFunctionLibrary::CreateAttributeModifier(CreateArgument);

	auto ConvertedModifier = StaticCastSharedPtr<FAttributeModifierBoolean>(NewModifier);
	if (ConvertedModifier.IsValid())
	{
		ConvertedModifier->SetFactor(InValue);
		return ConvertedModifier;
	}

	return nullptr; 
}

TSharedPtr<FAttributeModifierInteger> FCardAttributeModifierFunctionLibrary::CreateIntegerAttributeModifier(
	FName InName, int32 InValue, EModifierOperatorType OpType)
{
	FModifierCreateArgument CreateArgument;
	CreateArgument.DataType = ADT_Integer;
	CreateArgument.OperatorType = OpType;

	auto NewModifier = FAttributeFunctionLibrary::CreateAttributeModifier(CreateArgument);
	auto ConvertedModifier = StaticCastSharedPtr<FAttributeModifierInteger>(NewModifier);
	if (ConvertedModifier.IsValid())
	{
		ConvertedModifier->SetFactor(InValue);
		return ConvertedModifier;
	}

	return nullptr;
}

TSharedPtr<FAttributeModifierFloat> FCardAttributeModifierFunctionLibrary::CreateFloatAttributeModifier(FName InName,
	float InValue, EModifierOperatorType OpType)
{
	FModifierCreateArgument CreateArgument;
	CreateArgument.DataType = ADT_Float;
	CreateArgument.OperatorType = OpType;

	auto NewModifier = FAttributeFunctionLibrary::CreateAttributeModifier(CreateArgument);
	auto ConvertedModifier = StaticCastSharedPtr<FAttributeModifierFloat>(NewModifier);
	if (ConvertedModifier.IsValid())
	{
		ConvertedModifier->SetFactor(InValue);
		return ConvertedModifier;
	}

	return nullptr;
}
