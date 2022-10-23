#pragma once
#include "AttributeSystemCommon.h"
#include "Attribute/AttributeCommon.h"
#include "Attribute/AttributeBase.h"
#include "Modifier/AttributeModifierBase.h"

class ATTRIBUTESYSTEM_API FAttributeFunctionLibrary
{
public:
	static TSharedPtr<FAttributeBase> CreateAttribute(EAttributeDataType AttributeType, const FAttributeCreateArgument& InArgument);

	static TSharedPtr<FAttributeModifierBase> CreateAttributeModifier(EAttributeDataType ModifierType, const FModifierCreateArgument& InArgument);
};
