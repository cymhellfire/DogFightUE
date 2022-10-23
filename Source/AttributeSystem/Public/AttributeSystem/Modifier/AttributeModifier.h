#pragma once

#include "AttributeModifierBase.h"
#include "AttributeModifierCommon.h"

class FAttributeFunctionLibrary;

class ATTRIBUTESYSTEM_API FAttributeModifierBoolean : public FAttributeModifierBase<bool>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierBoolean()
		: FAttributeModifierBoolean(false)
	{}

	FAttributeModifierBoolean(bool InValue)
		: FAttributeModifierBase<bool>(InValue)
	{}

	virtual void UpdateModifiedValue() override;
};

class FAttributeModifierInteger : public FAttributeModifierNumeric<int32>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierInteger()
		: FAttributeModifierInteger(0, EModifierOperatorType::MOT_Equal)
	{}

	FAttributeModifierInteger(int32 InFactor, EModifierOperatorType InOpType)
		: FAttributeModifierNumeric<int32>(InFactor, InOpType)
	{}
};

class FAttributeModifierFloat : public FAttributeModifierNumeric<float>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierFloat()
		: FAttributeModifierFloat(0.f, EModifierOperatorType::MOT_Equal)
	{}

	FAttributeModifierFloat(float InFactor, EModifierOperatorType InOpType)
		: FAttributeModifierNumeric<float>(InFactor, InOpType)
	{}
};
