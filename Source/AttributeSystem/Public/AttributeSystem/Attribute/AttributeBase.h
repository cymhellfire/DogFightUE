#pragma once

#include "AttributeCommon.h"

template<typename T>
class FAttributeModifierBase;

template<typename T>
class ATTRIBUTESYSTEM_API FAttributeBase : public TSharedFromThis<FAttributeBase<T>>
{
public:
	virtual ~FAttributeBase(){}

	virtual FName GetName() const
	{
		return AttributeName;
	}

	virtual T GetRawValue() const
	{
		return Value;
	}

	virtual T GetValue() const;

	virtual void AddModifier(TSharedPtr<FAttributeModifierBase<T>> InModifier);

	virtual void RemoveModifier(TSharedPtr<FAttributeModifierBase<T>> InModifier);

	virtual TWeakPtr<FAttributeModifierBase<T>> GetLastModifier() const
	{
		return ModifierList.Num() > 0 ? ModifierList.Last() : nullptr;
	}

protected:
	FAttributeBase(const FAttributeCreateArgument& InArgument, T InValue)
	{
		AttributeName = InArgument.AttrName;
		Value = InValue;
	}

protected:
	FName AttributeName;

	int32 AttributeTag;

	T Value;

	TArray<TSharedPtr<FAttributeModifierBase<T>>> ModifierList; 
};
