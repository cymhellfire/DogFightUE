#pragma once
#include "AttributeSystemCommon.h"
#include "Attribute/AttributeCommon.h"
#include "Modifier/AttributeModifierBase.h"

class ATTRIBUTESYSTEM_API FAttributeFunctionLibrary
{
public:
	template<typename T>
	static TSharedPtr<FAttributeBase<T>> CreateAttribute(EAttributeDataType AttributeType, const FAttributeCreateArgument& InArgument);

	template<typename T>
	static TSharedPtr<FAttributeModifierBase<T>> CreateAttributeModifier(EAttributeDataType ModifierType);
};
