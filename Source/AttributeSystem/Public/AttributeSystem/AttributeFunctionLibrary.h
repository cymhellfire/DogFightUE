#pragma once

#include "Attribute/AttributeCommon.h"
#include "Attribute/AttributeBase.h"
#include "Modifier/AttributeModifierBase.h"

class ATTRIBUTESYSTEM_API FAttributeFunctionLibrary
{
public:
	static TSharedPtr<FAttributeBase> CreateAttribute(const FAttributeCreateArgument& InArgument);

	static TSharedPtr<FAttributeModifierBase> CreateAttributeModifier(const FModifierCreateArgument& InArgument);
};
