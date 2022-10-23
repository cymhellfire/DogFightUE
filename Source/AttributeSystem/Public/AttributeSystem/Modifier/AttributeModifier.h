#pragma once

#include "AttributeModifierBase.h"
#include "AttributeModifierCommon.h"

class FAttributeFunctionLibrary;

class ATTRIBUTESYSTEM_API FAttributeModifierBoolean : public TAttributeModifierBase<bool>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierBoolean(bool InValue)
		: TAttributeModifierBase<bool>(InValue)
	{}

	virtual void UpdateModifiedValue() override;
};

class FAttributeModifierInteger : public TAttributeModifierNumeric<int32>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierInteger()
		: TAttributeModifierNumeric<int32>()
	{}
};

class FAttributeModifierFloat : public TAttributeModifierNumeric<float>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierFloat()
		: TAttributeModifierNumeric<float>()
	{}
};
