#pragma once

#include "AttributeSystem/Modifier/AttributeModifierCommon.h"

template<typename T>
class FAttributeBase;

template<typename T>
class ATTRIBUTESYSTEM_API FAttributeModifierBase
{
public:
	virtual ~FAttributeModifierBase();

	virtual void Apply(TWeakPtr<FAttributeBase<T>> InAttribute);
	virtual void Remove();

	virtual void RegisterPreviousModifier(TWeakPtr<FAttributeModifierBase<T>> InModifier);
	virtual void ClearPreviousModifier();

	virtual void SetFactor(T InValue);

	T GetOriginValue() const;
	T GetModifiedValue();

	void MarkAsDirty()
	{
		bDirty = true;
	}

protected:
	FAttributeModifierBase(T InFactor);

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

	TWeakPtr<FAttributeBase<T>> ModifiedTarget;

	/** The modifier instance applied before. */
	TWeakPtr<FAttributeModifierBase<T>> PreviousModifier;
};

template<typename T>
class ATTRIBUTESYSTEM_API FAttributeModifierNumeric : public FAttributeModifierBase<T>
{
protected:
	FAttributeModifierNumeric(T InFactor, EModifierOperatorType InOpType)
		: FAttributeModifierBase<T>(InFactor)
		, OperatorType(InOpType)
	{}

	virtual void UpdateModifiedValue() override;

protected:
	EModifierOperatorType OperatorType;
};
