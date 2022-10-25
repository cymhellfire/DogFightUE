#pragma once

#include "AttributeCommon.h"
#include "AttributeSystem/AttributeSystemCommon.h"

class FAttributeModifierBase;
template<typename T>
class TAttributeModifierBase;

class ATTRIBUTESYSTEM_API FAttributeBase : public TSharedFromThis<FAttributeBase>
{
public:
	virtual ~FAttributeBase() {}

	virtual FName GetName() const = 0;

	virtual void AddModifier(TWeakPtr<FAttributeModifierBase> InModifier) = 0;
	virtual void RemoveModifier(TWeakPtr<FAttributeModifierBase> InModifier) = 0;

	virtual EAttributeDataType GetDataType() const
	{
		return DataType;
	}

protected:
	FAttributeBase()
		: DataType(ADT_None)
	{}

	EAttributeDataType DataType;
};

template<typename T>
class ATTRIBUTESYSTEM_API TAttributeBase : public FAttributeBase
{
public:
	virtual ~TAttributeBase() override {}

	virtual FName GetName() const override
	{
		return AttributeName;
	}

	virtual T GetRawValue() const
	{
		return Value;
	}

	virtual void SetRawValue(T InValue)
	{
		Value = InValue;
	}

	virtual T GetValue() const;

	virtual void AddModifier(TWeakPtr<FAttributeModifierBase> InModifier) override;

	virtual void RemoveModifier(TWeakPtr<FAttributeModifierBase> InModifier) override;

	virtual TWeakPtr<TAttributeModifierBase<T>> GetLastModifier() const
	{
		return ModifierList.Num() > 0 ? ModifierList.Last() : nullptr;
	}

protected:
	TAttributeBase(const FAttributeCreateArgument& InArgument, T InValue)
	{
		AttributeName = InArgument.AttrName;
		Value = InValue;
	}

protected:
	FName AttributeName;

	int32 AttributeTag;

	T Value;

	TArray<TSharedPtr<TAttributeModifierBase<T>>> ModifierList; 
};
