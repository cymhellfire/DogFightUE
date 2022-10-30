#pragma once
#include "ApplyRuleCommon.h"

class FAttributeBase;
class FApplyRuleParser;

class ATTRIBUTESYSTEM_API FApplyRuleBase : public TSharedFromThis<FApplyRuleBase>
{
public:
	virtual ~FApplyRuleBase() {}

	virtual bool CanApply(TSharedPtr<FAttributeBase> InAttribute) = 0;

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
