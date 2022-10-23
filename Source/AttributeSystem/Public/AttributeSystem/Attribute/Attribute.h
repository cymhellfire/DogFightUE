#pragma once

#include "AttributeBase.h"
#include "AttributeCommon.h"

class FAttributeFunctionLibrary;

class ATTRIBUTESYSTEM_API FAttributeBoolean : public FAttributeBase<bool>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeBoolean(const FAttributeCreateArgument& InArgument, bool InValue = false)
		: FAttributeBase<bool>(InArgument, InValue)
	{}
};

class ATTRIBUTESYSTEM_API FAttributeInteger : public FAttributeBase<int32>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeInteger(const FAttributeCreateArgument& InArgument, int32 InValue = 0)
		: FAttributeBase<int32>(InArgument, InValue)
	{}
};

class ATTRIBUTESYSTEM_API FAttributeFloat : public FAttributeBase<float>
{
	friend FAttributeFunctionLibrary;
protected:
	FAttributeFloat(const FAttributeCreateArgument& InArgument, float InValue = 0.f)
		: FAttributeBase<float>(InArgument, InValue)
	{}
};
