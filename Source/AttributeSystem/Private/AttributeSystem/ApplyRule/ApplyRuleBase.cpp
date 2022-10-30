#include "AttributeSystem/ApplyRule/ApplyRuleBase.h"

bool FApplyRuleGroup::CanApply(TSharedPtr<FAttributeBase> InAttribute)
{
	switch (Type)
	{
	case ARGT_PassAll:
		// All children rules must pass
		for (auto Rule : ChildApplyRules)
		{
			if (!Rule->CanApply(InAttribute))
			{
				return false;
			}
		}
		return true;
	case ARGT_PassOne:
		// One of children rules pass
		for (auto Rule : ChildApplyRules)
		{
			if (Rule->CanApply(InAttribute))
			{
				return true;
			}
		}
		return false;
	default: ;
	}

	return false;
}

TSharedPtr<FApplyRuleGroup> FApplyRuleGroup::AddChild(TSharedPtr<FApplyRuleBase> InRule)
{
	if (!ChildApplyRules.Contains(InRule))
	{
		ChildApplyRules.Add(InRule);
	}

	return SharedThis(this);
}

TSharedPtr<FApplyRuleGroup> FApplyRuleGroup::RemoveChild(TSharedPtr<FApplyRuleBase> InRule)
{
	if (ChildApplyRules.Contains(InRule))
	{
		ChildApplyRules.Remove(InRule);
	}

	return SharedThis(this);
}
