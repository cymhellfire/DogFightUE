#pragma once
#include "ApplyRuleCommon.h"
#include "AttributeSystem/AttributeSystemCommon.h"

class FAttributeBase;
class FApplyRuleParser;

class ATTRIBUTESYSTEM_API FApplyRuleBase : public TSharedFromThis<FApplyRuleBase>
{
public:
	virtual ~FApplyRuleBase() {}

	virtual bool CanApply(TSharedPtr<FAttributeBase> InAttribute) = 0;

	virtual void GetDesiredName(TArray<FName>& OutNames) = 0;

	virtual void GetDesiredDataType(TArray<EAttributeDataType>& OutTypes) = 0;

	/**
	 * @brief Check if there is one specified name can fit the apply rule (return false when there are multiple existing).
	 * @param ApplyRule Apply rule to check with.
	 * @param OutName Name that the apply rule needed.
	 * @return Whether the name exists.
	 */
	static bool HasDesiredName(TSharedPtr<FApplyRuleBase> ApplyRule, FName& OutName);

	/**
	 * @brief Check if there is one specified data type can fit the apply rule (return false when there are multiple existing).
	 * @param ApplyRule Apply rule to check with.
	 * @param DataType Data type that the apply rule needed.
	 * @return Whether the data type exists.
	 */
	static bool HasDesiredDataType(TSharedPtr<FApplyRuleBase> ApplyRule, EAttributeDataType& DataType);

protected:
	FApplyRuleBase() {}
};

/**
 * @brief Rule group that contains multiple apply rules as children.
 */
class ATTRIBUTESYSTEM_API FApplyRuleGroup : public FApplyRuleBase
{
	friend FApplyRuleParser;
public:
	virtual ~FApplyRuleGroup() override {}

	virtual bool CanApply(TSharedPtr<FAttributeBase> InAttribute) override;

	virtual void GetDesiredName(TArray<FName>& OutNames) override;

	virtual void GetDesiredDataType(TArray<EAttributeDataType>& OutTypes) override;

	void SetGroupType(EApplyRuleGroupType InType)
	{
		Type = InType;
	}

	EApplyRuleGroupType GetGroupType() const
	{
		return Type;
	}

	TSharedPtr<FApplyRuleGroup> AddChild(TSharedPtr<FApplyRuleBase> InRule);
	TSharedPtr<FApplyRuleGroup> RemoveChild(TSharedPtr<FApplyRuleBase> InRule);

protected:
	FApplyRuleGroup()
	: FApplyRuleGroup(ARGT_PassAll)
	{}

	FApplyRuleGroup(EApplyRuleGroupType InType)
		: FApplyRuleBase()
		, Type(InType)
	{}

	EApplyRuleGroupType Type;

	TArray<TSharedPtr<FApplyRuleBase>> ChildApplyRules;
};
