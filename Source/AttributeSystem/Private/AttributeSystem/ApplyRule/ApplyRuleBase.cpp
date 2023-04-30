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

void FApplyRuleGroup::GetDesiredName(TArray<FName>& OutNames)
{
	for (auto Child : ChildApplyRules)
	{
		if (Child.IsValid())
		{
			Child->GetDesiredName(OutNames);
		}
	}
}

void FApplyRuleGroup::GetDesiredDataType(TArray<EAttributeDataType>& OutTypes)
{
	for (auto Child : ChildApplyRules)
	{
		if (Child.IsValid())
		{
			Child->GetDesiredDataType(OutTypes);
		}
	}
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

bool FApplyRuleBase::HasDesiredName(TSharedPtr<FApplyRuleBase> ApplyRule, FName& OutName)
{
	TArray<FName> DesiredNames;
	ApplyRule->GetDesiredName(DesiredNames);

	// Only single name is acceptable
	if (DesiredNames.Num() == 1)
	{
		OutName = DesiredNames[0];
		return true;
	}

	return false;
}

bool FApplyRuleBase::HasDesiredDataType(TSharedPtr<FApplyRuleBase> ApplyRule, EAttributeDataType& DataType)
{
	TArray<EAttributeDataType> DataTypes;
	ApplyRule->GetDesiredDataType(DataTypes);

	// Only single data type is acceptable
	if (DataTypes.Num() == 1)
	{
		DataType = DataTypes[0];
		return true;
	}

	return false;
}
