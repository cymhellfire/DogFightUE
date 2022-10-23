#pragma once

#include "AttributeBase.h"
#include "AttributeCommon.h"

class FAttributeFunctionLibrary;

class ATTRIBUTESYSTEM_API FAttributeBoolean : public TAttributeBase<bool>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeBoolean(const FAttributeCreateArgument& InArgument, bool InValue = false)
		: TAttributeBase<bool>(InArgument, InValue)
	{
		DataType = EAttributeDataType::ADT_Boolean;
	}
};

class ATTRIBUTESYSTEM_API FAttributeInteger : public TAttributeBase<int32>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeInteger(const FAttributeCreateArgument& InArgument, int32 InValue = 0)
		: TAttributeBase<int32>(InArgument, InValue)
	{
		DataType = EAttributeDataType::ADT_Integer;
	}
};

class ATTRIBUTESYSTEM_API FAttributeFloat : public TAttributeBase<float>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeFloat(const FAttributeCreateArgument& InArgument, float InValue = 0.f)
		: TAttributeBase<float>(InArgument, InValue)
	{
		DataType = EAttributeDataType::ADT_Float;
	}
};
