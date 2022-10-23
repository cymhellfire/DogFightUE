#pragma once

#include "AttributeModifierBase.h"
#include "AttributeModifierCommon.h"

class FAttributeFunctionLibrary;

class ATTRIBUTESYSTEM_API FAttributeModifierBoolean : public TAttributeModifierBase<bool>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierBoolean(const FModifierCreateArgument& InArgument, bool InValue = false)
		: TAttributeModifierBase<bool>(InArgument, InValue)
	{
		DataType = EAttributeDataType::ADT_Boolean;
	}

	virtual void UpdateModifiedValue() override;
};

class FAttributeModifierInteger : public TAttributeModifierNumeric<int32>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierInteger(const FModifierCreateArgument& InArgument, int32 InValue = 0)
		: TAttributeModifierNumeric<int32>(InArgument, InValue)
	{
		DataType = EAttributeDataType::ADT_Integer;
	}
};

class FAttributeModifierFloat : public TAttributeModifierNumeric<float>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeModifierFloat(const FModifierCreateArgument& InArgument, float InValue = 0.f)
		: TAttributeModifierNumeric<float>(InArgument, InValue)
	{
		DataType = EAttributeDataType::ADT_Float;
	}
};
