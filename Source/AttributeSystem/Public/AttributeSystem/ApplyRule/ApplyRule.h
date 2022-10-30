#pragma once

#include "ApplyRuleBase.h"
#include "AttributeSystem/AttributeSystemCommon.h"

/**
 * @brief Apply rule that matching attribute name.
 */
class ATTRIBUTESYSTEM_API FNameMatchApplyRule : public FApplyRuleBase
{
	friend FApplyRuleParser;
public:
	virtual bool CanApply(TSharedPtr<FAttributeBase> InAttribute) override;

	void SetMatchName(FName InName)
	{
		MatchName = InName;
	}

	FName GetMatchName() const
	{
		return MatchName;
	}

protected:
	FNameMatchApplyRule()
	: FNameMatchApplyRule(NAME_None)
	{}

	FNameMatchApplyRule(FName InName)
		: FApplyRuleBase()
		, MatchName(InName)
	{}

	FName MatchName;
};

/**
 * @brief Apply rule that matching attribute tags.
 */
class ATTRIBUTESYSTEM_API FTagMatchApplyRule : public FApplyRuleBase
{
	friend FApplyRuleParser;
public:
	virtual bool CanApply(TSharedPtr<FAttributeBase> InAttribute) override;

	void SetMatchMethod(ETagMatchMethod InMethod)
	{
		MatchMethod = InMethod;
	}

protected:
	FTagMatchApplyRule()
	: FTagMatchApplyRule(TSet<FName>(), TMM_ContainAll)
	{}

	FTagMatchApplyRule(TSet<FName> InTags, ETagMatchMethod InMethod)
		: FApplyRuleBase()
		, MatchTags(InTags)
		, MatchMethod(InMethod)
	{}

	TSet<FName> MatchTags;

	ETagMatchMethod MatchMethod;
};

/**
 * @brief Apply rule that matching attribute data type.
 */
class ATTRIBUTESYSTEM_API FDataTypeMatchApplyRule : public FApplyRuleBase
{
	friend FApplyRuleParser;
public:
	virtual bool CanApply(TSharedPtr<FAttributeBase> InAttribute) override;

	void SetMatchDataType(EAttributeDataType InType)
	{
		MatchType = InType;
	}

protected:
	FDataTypeMatchApplyRule()
		: FDataTypeMatchApplyRule(ADT_None)
	{}

	FDataTypeMatchApplyRule(EAttributeDataType InType)
		: FApplyRuleBase()
		, MatchType(InType)
	{}

	EAttributeDataType MatchType;
};
