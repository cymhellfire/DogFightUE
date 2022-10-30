#include "AttributeSystem/ApplyRule/ApplyRule.h"

#include "AttributeSystem/Attribute/AttributeBase.h"

bool FNameMatchApplyRule::CanApply(TSharedPtr<FAttributeBase> InAttribute)
{
	return InAttribute->GetName() == MatchName;
}

bool FTagMatchApplyRule::CanApply(TSharedPtr<FAttributeBase> InAttribute)
{
	switch(MatchMethod)
	{
	case TMM_ContainAll:
		// Check all tags
		for (auto Tag : MatchTags)
		{
			if (!InAttribute->HasTag(Tag))
			{
				return false;
			}
		}
		return true;
	case TMM_ContainOne:
		for (auto Tag : MatchTags)
		{
			if (InAttribute->HasTag(Tag))
			{
				return true;
			}
		}
		break;
	default: ;
	}

	return false;
}

bool FDataTypeMatchApplyRule::CanApply(TSharedPtr<FAttributeBase> InAttribute)
{
	return InAttribute->GetDataType() == MatchType;
}
