#include "AttributeSystem/AttributeFunctionLibrary.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Modifier/AttributeModifier.h"

TSharedPtr<FAttributeBase> FAttributeFunctionLibrary::CreateAttribute(EAttributeDataType AttributeType, const FAttributeCreateArgument& InArgument)
{
	switch (AttributeType)
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

TSharedPtr<FAttributeModifierBase> FAttributeFunctionLibrary::CreateAttributeModifier(EAttributeDataType ModifierType, const FModifierCreateArgument& InArgument)
{
	switch (ModifierType)
	{
	case ADT_Boolean:
		return MakeShareable(new FAttributeModifierBoolean(InArgument));
	case ADT_Integer:
		return MakeShareable(new FAttributeModifierInteger(InArgument));
	case ADT_Float:
		return MakeShareable(new FAttributeModifierFloat(InArgument));
	case ADT_None:
	default:
		return nullptr;
	}
}
