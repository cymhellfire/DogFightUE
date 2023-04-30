#include "AttributeSystem/ApplyRule/ApplyRule.h"

#include "AttributeSystem/Attribute/AttributeBase.h"

bool FNameMatchApplyRule::CanApply(TSharedPtr<FAttributeBase> InAttribute)
{
	return InAttribute->GetName() == MatchName;
}

void FNameMatchApplyRule::GetDesiredName(TArray<FName>& OutNames)
{
	OutNames.AddUnique(MatchName);
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

void FDataTypeMatchApplyRule::GetDesiredDataType(TArray<EAttributeDataType>& OutTypes)
{
	OutTypes.AddUnique(MatchType);
}
