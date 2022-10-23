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

TSharedPtr<FAttributeModifierBase> FAttributeFunctionLibrary::CreateAttributeModifier(EAttributeDataType ModifierType)
{
	switch (ModifierType)
	{
	case ADT_Boolean:
		return MakeShareable(new FAttributeModifierBoolean(false, false));
	case ADT_Integer:
		return MakeShareable(new FAttributeModifierInteger);
	case ADT_Float:
		return MakeShareable(new FAttributeModifierFloat);
	case ADT_None:
	default:
		return nullptr;
	}
}
