#include "AttributeSystem/ApplyRule/ApplyRuleParser.h"

#include "AttributeSystem.h"
#include "AttributeSystem/ApplyRule/ApplyRule.h"
#include "AttributeSystem/ApplyRule/ApplyRuleBase.h"

// Remove heading N characters from string.
#define CONSUME_HEADING(InString, Count)		InString = InString.Mid(Count, FMath::Max(InString.Len() - Count, 0))

#define TAG_MAX_LOOP		1000

TArray<TCHAR> FApplyRuleParser::ApplyRuleEndingCharacters = {',', '(', ')', '[', ']'};

/**
 * Parse rules:
 * NameMatch -> "Name=XXX"
 * DataTypeMatch -> "DataType=XXX"
 * TagMatch(ContainAll) -> "Tag(Tag1,Tag2)"
 * TagMatch(ContainOne) -> "Tag[Tag1,Tag2]"
 * RuleGroup(PassAll) -> "(Rule1,Rule2)"
 * RuleGroup(PassOne) -> "[Rule1,Rule2]"
 */
TSharedPtr<FApplyRuleBase> FApplyRuleParser::ParseApplyRuleFromString(FString Source)
{
	if (Source.IsEmpty())
	{
		return nullptr;
	}

	FString ParseString = FString(Source);
	return ParseApplyRule(ParseString);
}

TSharedPtr<FApplyRuleBase> FApplyRuleParser::ParseApplyRule(FString& Source)
{
	TSharedPtr<FApplyRuleBase> Result;
	// Check for rule group
	if (Source.StartsWith("(") || Source.StartsWith("["))
	{
		Result = ParseApplyRuleGroup(Source);
	}
	// Check for name
	else if (Source.StartsWith("Name="))
	{
		Result = ParseNameMatchRule(Source);
	}
	// Check for data type
	else if (Source.StartsWith("DataType="))
	{
		Result = ParseDataTypeMatchRule(Source);
	}
	else if (Source.StartsWith("Tag"))
	{
		Result = ParseTagMatchRule(Source);
	}
	else
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Invalid string to parse [%s]"), *Source);
		return nullptr;
	}

	return Result;
}

TSharedPtr<FApplyRuleBase> FApplyRuleParser::ParseApplyRuleGroup(FString& Source)
{
	// Check matching mode
	EApplyRuleGroupType RuleGroupMatchMethod;
	if (Source.StartsWith("("))
	{
		RuleGroupMatchMethod = ARGT_PassAll;
	}
	else if (Source.StartsWith("["))
	{
		RuleGroupMatchMethod = ARGT_PassOne;
	}
	else
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Failed to parser rule group match method from [%s]"), *Source);
		return nullptr;
	}

	// Consume the heading symbol
	CONSUME_HEADING(Source, 1);

	// Parse all child rules
	TArray<TSharedPtr<FApplyRuleBase>> ChildrenRules;
	bool bFinished = false;
	while (!bFinished)
	{
		auto NewRule = ParseApplyRule(Source);

		// Interrupt creation if child apply rule failed to be parsed
		if (NewRule == nullptr || !NewRule.IsValid())
		{
			return nullptr;
		}
		ChildrenRules.Add(NewRule);

		// Source string shouldn't empty now
		if (Source.IsEmpty())
		{
			UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Source string empty when parsing apply rule group."));
			return nullptr;
		}

		// Check whether to parser next child
		int32 EndingIndex = -1;
		EApplyRuleEndingCharacter EndingCharacter = FindNextEndingChar(Source, EndingIndex);
		switch (EndingCharacter)
		{
		case EC_Comma:
			// Normal case do nothing
			break;
		case EC_CloseParen:
			if (RuleGroupMatchMethod == ARGT_PassOne)
			{
				UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Expected ']' symbol to finish apply rule group string [%s]."), *Source);
				return nullptr;
			}
			bFinished = true;
			break;
		case EC_CloseBracket:
			if (RuleGroupMatchMethod == ARGT_PassAll)
			{
				UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Expected ')' symbol to finish apply rule group string [%s]."), *Source);
				return nullptr;
			}
			bFinished = true;
			break;
		// Invalid cases
		case EC_None:
		case EC_OpenParen:
		case EC_OpenBracket: 
		default:
			UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Invalid operator while parsing apply rule group [%s]."), *Source);
			return nullptr;
		}

		// Remove the ending character
		CONSUME_HEADING(Source, 1);
		if (Source.IsEmpty())
		{
			break;
		}
	}

	// Create apply rule group
	TSharedPtr<FApplyRuleGroup> NewRuleGroup = MakeShareable(new FApplyRuleGroup(RuleGroupMatchMethod));
	// Set children rules
	for (auto Rule : ChildrenRules)
	{
		NewRuleGroup->AddChild(Rule);
	}

	return NewRuleGroup;
}

TSharedPtr<FApplyRuleBase> FApplyRuleParser::ParseNameMatchRule(FString& Source)
{
	if (!Source.StartsWith("Name="))
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Failed to parser string [%s] as NameMatchRule."), *Source);
		return nullptr;
	}

	// Remove heading keyword
	CONSUME_HEADING(Source, 5);

	int32 EndingIndex = -1;
	FString MatchName;
	// Get ending character
	if (FindNextEndingChar(Source, EndingIndex) != EC_None)
	{
		MatchName = Source.Mid(0, EndingIndex);

		CONSUME_HEADING(Source, EndingIndex);
	}
	else
	{
		MatchName = Source;

		// All content has been used
		Source.Empty();
	}

	return MakeShareable(new FNameMatchApplyRule(FName(MatchName)));
}

TSharedPtr<FApplyRuleBase> FApplyRuleParser::ParseTagMatchRule(FString& Source)
{
	if (!Source.StartsWith("Tag"))
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Failed to parser string [%s] as TagMatchRule."), *Source);
		return nullptr;
	}

	// Remove heading keyword
	CONSUME_HEADING(Source, 3);

	// Check matching mode
	ETagMatchMethod TagMatchMethod;
	if (Source.StartsWith("("))
	{
		TagMatchMethod = TMM_ContainAll;
	}
	else if (Source.StartsWith("["))
	{
		TagMatchMethod = TMM_ContainOne;
	}
	else
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Failed to parser tag match method from [%s]"), *Source);
		return nullptr;
	}

	// Remove the open bracket
	CONSUME_HEADING(Source, 1);

	// Parse following tags
	TSet<FName> TagList;
	int32 LoopCounter = 0;
	bool bFinished = false;
	while (!bFinished && LoopCounter <= TAG_MAX_LOOP)
	{
		int32 EndingIndex = 0;
		EApplyRuleEndingCharacter EndingType = FindNextEndingChar(Source, EndingIndex);
		// Check ending type
		switch (EndingType)
		{
		case EC_CloseParen:
			// Check with tag match method
			if (TagMatchMethod == TMM_ContainOne)
			{
				UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Expected ']' symbol to finish tag match apply rule string [%s]."), *Source);
				return nullptr;
			}
			bFinished = true;
			goto ParseTag;
		case EC_CloseBracket:
			// Check with tag match method
			if (TagMatchMethod == TMM_ContainAll)
			{
				UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Expected ')' symbol to finish tag match apply rule string [%s]."), *Source);
				return nullptr;
			}
			bFinished = true;
		case EC_Comma:
			// Continue to parser next tag
ParseTag:
			{
				FString NewTag = Source.Mid(0, EndingIndex);
				// Consume one more character to ensure the ending character can be removed
				CONSUME_HEADING(Source, EndingIndex + 1);

				TagList.Add(FName(NewTag));
			}
			break;
		case EC_None:
			// Use left string as one tag
			{
				FString NewTag = Source;
				Source.Empty();

				TagList.Add(FName(NewTag));
			}
			break;
		// Invalid cases
		case EC_OpenParen:
		case EC_OpenBracket:
		default:
			UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Invalid operator in tag match apply rule [%s]."), *Source);
			return nullptr;
		}

		if (Source.IsEmpty())
		{
			break;
		}

		LoopCounter++;
	}

	// Report the infinite looping issue
	if (LoopCounter >= TAG_MAX_LOOP)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Tag parsing meet the looping limitation, there might be some error."));
	}

	return MakeShareable(new FTagMatchApplyRule(TagList, TagMatchMethod));
}

TSharedPtr<FApplyRuleBase> FApplyRuleParser::ParseDataTypeMatchRule(FString& Source)
{
	if (!Source.StartsWith("DataType="))
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Failed to parser string [%s] as DataTypeMatchRule."), *Source);
		return nullptr;
	}

	// Remove heading keyword
	CONSUME_HEADING(Source, 9);

	int32 EndingIndex = -1;
	FString DataTypeStr;
	// Get ending character
	if (FindNextEndingChar(Source, EndingIndex) != EC_None)
	{
		DataTypeStr = Source.Mid(0, EndingIndex);

		CONSUME_HEADING(Source, EndingIndex);
	}
	else
	{
		DataTypeStr = Source;

		// All content has been used
		Source.Empty();
	}

	EAttributeDataType DataType = ADT_None;
	// Parse to data type
	if (DataTypeStr == TEXT("Boolean"))
	{
		DataType = ADT_Boolean;
	}
	else if (DataTypeStr == TEXT("Integer"))
	{
		DataType = ADT_Integer;
	}
	else if (DataTypeStr == TEXT("Float"))
	{
		DataType = ADT_Float;
	}
	else
	{
		// Unknown data type is not permitted
		UE_LOG(LogAttributeSystem, Error, TEXT("[ApplyRuleParser] Invalid data type [%s]."), *DataTypeStr);
		return nullptr;
	}

	return MakeShareable(new FDataTypeMatchApplyRule(DataType));
}

EApplyRuleEndingCharacter FApplyRuleParser::FindNextEndingChar(const FString& InString, int32& Index)
{
	if (InString.IsEmpty())
	{
		return EC_None;
	}

	EApplyRuleEndingCharacter Result = EC_None;
	int32 FirstIndex = InString.Len();
	for (int32 CharIndex = 0; CharIndex < ApplyRuleEndingCharacters.Num(); ++CharIndex)
	{
		int32 CurIndex = 0;
		if (InString.FindChar(ApplyRuleEndingCharacters[CharIndex], CurIndex))
		{
			// Smaller index has higher priority
			if (CurIndex < FirstIndex)
			{
				FirstIndex = CurIndex;
				Result = static_cast<EApplyRuleEndingCharacter>(CharIndex + 1);
			}
		}
	}

	Index = FirstIndex;
	return Result;
}
