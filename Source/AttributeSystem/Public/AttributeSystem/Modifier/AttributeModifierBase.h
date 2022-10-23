#pragma once

#include "AttributeSystem/AttributeSystemCommon.h"
#include "AttributeSystem/Modifier/AttributeModifierCommon.h"

class FAttributeBase;
template<typename T>
class TAttributeBase;

class ATTRIBUTESYSTEM_API FAttributeModifierBase
{
public:
	virtual ~FAttributeModifierBase() {}

	virtual void Apply(TWeakPtr<FAttributeBase> InAttribute) = 0;
	virtual void Remove() = 0;

	virtual EAttributeDataType GetDataType() const
	{
		return DataType;
	}

protected:
	FAttributeModifierBase()
		: DataType(EAttributeDataType::ADT_None)
	{}

	EAttributeDataType DataType;
};

template<typename T>
class ATTRIBUTESYSTEM_API TAttributeModifierBase : public FAttributeModifierBase
{
public:
	virtual ~TAttributeModifierBase() override;

	virtual void Apply(TWeakPtr<FAttributeBase> InAttribute) override;
	virtual void Remove() override;

	virtual void RegisterPreviousModifier(TWeakPtr<TAttributeModifierBase<T>> InModifier);
	virtual void ClearPreviousModifier();

	virtual void SetFactor(T InValue);

	T GetOriginValue() const;
	T GetModifiedValue();

	void MarkAsDirty()
	{
		bDirty = true;
	}

protected:
	TAttributeModifierBase(const FModifierCreateArgument& InArgument, T InValue);

	virtual void UpdateModifiedValue();

	virtual void OnPreviousModifierDirty();

public:
	DECLARE_MULTICAST_DELEGATE(FAttributeModifierSimpleSignature)
	FAttributeModifierSimpleSignature OnModifierDirty;

protected:
	uint8 bDirty:1;

	/** Factor value to apply. */
	T ModifyFactor;

	T ModifiedValue;

	FDelegateHandle PreviousModifierDirtyHandle;

	TWeakPtr<TAttributeBase<T>> ModifiedTarget;

	/** The modifier instance applied before. */
	TWeakPtr<TAttributeModifierBase<T>> PreviousModifier;
};

template<typename T>
class ATTRIBUTESYSTEM_API TAttributeModifierNumeric : public TAttributeModifierBase<T>
{
protected:
	TAttributeModifierNumeric(const FModifierCreateArgument& InArgument, T InValue)
		: TAttributeModifierBase<T>(InArgument, InValue)
		, OperatorType(InArgument.OperatorType)
	{}

	virtual void UpdateModifiedValue() override;

protected:
	EModifierOperatorType OperatorType;
};
