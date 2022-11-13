#pragma once

#include "AttributeBase.h"
#include "AttributeCommon.h"

class FAttributeFunctionLibrary;

class ATTRIBUTESYSTEM_API FAttributeBoolean : public TAttributeBase<bool>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeBoolean(const FAttributeCreateArgument& InArgument)
		: TAttributeBase<bool>(InArgument, InArgument.InitBooleanValue)
	{
		DataType = EAttributeDataType::ADT_Boolean;
	}
};

class ATTRIBUTESYSTEM_API FAttributeInteger : public TAttributeBase<int32>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeInteger(const FAttributeCreateArgument& InArgument)
		: TAttributeBase<int32>(InArgument, InArgument.InitIntegerValue)
	{
		DataType = EAttributeDataType::ADT_Integer;
	}
};

class ATTRIBUTESYSTEM_API FAttributeFloat : public TAttributeBase<float>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeFloat(const FAttributeCreateArgument& InArgument)
		: TAttributeBase<float>(InArgument, InArgument.InitFloatValue)
	{
		DataType = EAttributeDataType::ADT_Float;
	}
};
