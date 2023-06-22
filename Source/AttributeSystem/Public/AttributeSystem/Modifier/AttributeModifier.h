#pragma once

#include "AttributeModifierBase.h"
#include "AttributeModifierCommon.h"

class FAttributeFunctionLibrary;

class ATTRIBUTESYSTEM_API FAttributeModifierBoolean : public TAttributeModifierBase<bool>
{
	friend FAttributeFunctionLibrary;
public:
	virtual FString GetEffectString() const override;

	virtual FText GetLocalizedEffectString() override;
protected:
	FAttributeModifierBoolean(const FModifierCreateArgument& InArgument)
		: TAttributeModifierBase<bool>(InArgument, InArgument.InitBooleanValue)
	{
		DataType = EAttributeDataType::ADT_Boolean;
	}

	virtual void UpdateModifiedValue() override;
};

class FAttributeModifierInteger : public TAttributeModifierNumeric<int32>
{
	friend FAttributeFunctionLibrary;
public:
	virtual FString GetEffectString() const override;

	virtual FText GetLocalizedEffectString() override;
protected:
	FAttributeModifierInteger(const FModifierCreateArgument& InArgument)
		: TAttributeModifierNumeric<int32>(InArgument, InArgument.InitIntegerValue)
	{
		DataType = EAttributeDataType::ADT_Integer;
	}
};

class FAttributeModifierFloat : public TAttributeModifierNumeric<float>
{
	friend FAttributeFunctionLibrary;
public:
	virtual FString GetEffectString() const override;

	virtual FText GetLocalizedEffectString() override;
protected:
	FAttributeModifierFloat(const FModifierCreateArgument& InArgument)
		: TAttributeModifierNumeric<float>(InArgument, InArgument.InitFloatValue)
	{
		DataType = EAttributeDataType::ADT_Float;
	}
};
