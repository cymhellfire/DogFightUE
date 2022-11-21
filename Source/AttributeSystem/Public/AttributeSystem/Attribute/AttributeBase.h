#pragma once

#include "AttributeCommon.h"
#include "AttributeSystem/AttributeSystemCommon.h"

class FAttributeModifierBase;
template<typename T>
class TAttributeModifierBase;

class ATTRIBUTESYSTEM_API FAttributeBase : public TSharedFromThis<FAttributeBase>
{
	friend class FAttributeWrapperObjectHelper;
public:
	virtual ~FAttributeBase() {}

	virtual FName GetName() const
	{
		return AttributeName;
	}

	void AddTag(FName InTag)
	{
		AttributeTags.Add(InTag);
	}

	void AddTags(const TArray<FString>& InTags)
	{
		for (auto Tag : InTags)
		{
			AttributeTags.Add(FName(Tag));
		}
	}

	void RemoveTag(FName InTag)
	{
		AttributeTags.Remove(InTag);
	}

	void RemoveTags(const TArray<FString>& InTags)
	{
		for (auto Tag : InTags)
		{
			AttributeTags.Remove(FName(Tag));
		}
	}

	void ClearTags()
	{
		AttributeTags.Empty();
	}

	bool HasTag(FName InTag) const
	{
		return AttributeTags.Contains(InTag);
	}

	bool HasAllTags(const TArray<FName>& InTags) const
	{
		for (auto Tag : InTags)
		{
			if (!HasTag(Tag))
			{
				return false;
			}
		}

		return true;
	}

	bool HasAnyTag(const TArray<FName>& InTags) const
	{
		for (auto Tag: InTags)
		{
			if (HasTag(Tag))
			{
				return true;
			}
		}

		return false;
	}

	virtual void AddModifier(TSharedPtr<FAttributeModifierBase> InModifier) = 0;
	virtual void RemoveModifier(TWeakPtr<FAttributeModifierBase> InModifier) = 0;

	virtual EAttributeDataType GetDataType() const
	{
		return DataType;
	}
protected:
	FAttributeBase(const FAttributeCreateArgument& InArgument)
		: AttributeName(InArgument.AttrName)
		,DataType(ADT_None)
	{
		// Import tags
		if (InArgument.Tags.Num() > 0)
		{
			AddTags(InArgument.Tags);
		}
	}

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FAttributeValueChangeSignature, TSharedPtr<FAttributeBase>)
	FAttributeValueChangeSignature OnValueChanged;

	FName AttributeName;

	TSet<FName> AttributeTags;

	EAttributeDataType DataType;
};

template<typename T>
class ATTRIBUTESYSTEM_API TAttributeBase : public FAttributeBase
{
public:
	virtual ~TAttributeBase() override {}

	virtual T GetRawValue() const
	{
		return Value;
	}

	virtual void SetRawValue(T InValue)
	{
		Value = InValue;

		// Trigger delegate
		OnValueChanged.Broadcast(SharedThis(this));
	}

	virtual T GetValue() const;

	virtual void AddModifier(TSharedPtr<FAttributeModifierBase> InModifier) override;

	virtual void RemoveModifier(TWeakPtr<FAttributeModifierBase> InModifier) override;

	virtual TWeakPtr<TAttributeModifierBase<T>> GetLastModifier() const
	{
		return ModifierList.Num() > 0 ? ModifierList.Last() : nullptr;
	}

protected:
	TAttributeBase(const FAttributeCreateArgument& InArgument, T InValue)
		: FAttributeBase(InArgument)
		, Value(InValue)
	{}

protected:
	T Value;

	TArray<TSharedPtr<TAttributeModifierBase<T>>> ModifierList; 
};
