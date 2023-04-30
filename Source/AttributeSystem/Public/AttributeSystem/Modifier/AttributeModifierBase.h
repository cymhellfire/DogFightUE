#pragma once

#include "AttributeSystem/AttributeSystemCommon.h"
#include "AttributeSystem/ApplyRule/ApplyRuleBase.h"
#include "AttributeSystem/Modifier/AttributeModifierCommon.h"

class FAttributeBase;
template<typename T>
class TAttributeBase;
class FApplyRuleBase;

class ATTRIBUTESYSTEM_API FAttributeModifierBase : public TSharedFromThis<FAttributeModifierBase>
{
public:
	virtual ~FAttributeModifierBase() {}

	virtual bool Apply(TWeakPtr<FAttributeBase> InAttribute) = 0;
	virtual void Remove() = 0;
	virtual void RemoveFromTarget(TSharedPtr<FAttributeBase>& OutTarget) = 0;

	virtual void SetApplyRule(TSharedPtr<FApplyRuleBase> InRule)
	{
		ApplyRule = InRule;
	}

	virtual EAttributeDataType GetDataType() const
	{
		return DataType;
	}

	virtual FString GetEffectString() const = 0;

	virtual bool CanApply(TSharedPtr<FAttributeBase> InAttribute) const
	{
		if (ApplyRule.IsValid())
		{
			return ApplyRule->CanApply(InAttribute);
		}

		// Return true as default if there is no rule
		return true;
	}

	/**
	 * @brief Get the desired attribute name specified by the apply rules.
	 * @param OutName Desired attribute name of this modifier.
	 * @return Whether the desired name exists.
	 */
	virtual bool GetDesiredName(FName& OutName) const
	{
		if (ApplyRule.IsValid())
		{
			return FApplyRuleBase::HasDesiredName(ApplyRule, OutName);
		}

		return false;
	}

	/**
	 * @brief Get the desired attribute data type specified by the apply rules.
	 * @param OutType Desired data type of this modifier.
	 * @return Whether the desired data type exists.
	 */
	virtual bool GetDesiredDataType(EAttributeDataType& OutType) const
	{
		bool Result = false;
		if (ApplyRule.IsValid())
		{
			Result = FApplyRuleBase::HasDesiredDataType(ApplyRule, OutType);
		}

		// Use the modifier data type as default
		if (!Result)
		{
			OutType = DataType;
		}
		return true;
	}

protected:
	FAttributeModifierBase()
		: DataType(EAttributeDataType::ADT_None)
	{}

	EAttributeDataType DataType;

	TSharedPtr<FApplyRuleBase> ApplyRule;
};

template<typename T>
class ATTRIBUTESYSTEM_API TAttributeModifierBase : public FAttributeModifierBase
{
public:
	virtual ~TAttributeModifierBase() override;

	virtual bool Apply(TWeakPtr<FAttributeBase> InAttribute) override;
	virtual void Remove() override;
	virtual void RemoveFromTarget(TSharedPtr<FAttributeBase>& OutTarget) override;

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
