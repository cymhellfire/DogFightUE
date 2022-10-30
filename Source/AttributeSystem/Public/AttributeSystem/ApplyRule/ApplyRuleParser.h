#pragma once

class FApplyRuleBase;

enum EApplyRuleEndingCharacter
{
	EC_None,
	EC_Comma,				// ','
	EC_OpenParen,			// '('
	EC_CloseParen,			// ')'
	EC_OpenBracket,			// '['
	EC_CloseBracket,		// ']'
};

class ATTRIBUTESYSTEM_API FApplyRuleParser
{
public:
	/**
	 * @brief Parse apply rule from a string.
	 * @param Source		Source string to parse
	 * @return New created apply rule
	 */
	static TSharedPtr<FApplyRuleBase> ParseApplyRuleFromString(FString Source);

protected:
	static TSharedPtr<FApplyRuleBase> ParseApplyRule(FString& Source);
	static TSharedPtr<FApplyRuleBase> ParseApplyRuleGroup(FString& Source);
	static TSharedPtr<FApplyRuleBase> ParseNameMatchRule(FString& Source);
	static TSharedPtr<FApplyRuleBase> ParseTagMatchRule(FString& Source);
	static TSharedPtr<FApplyRuleBase> ParseDataTypeMatchRule(FString& Source);

	/**
	 * @brief Find the next apply rule ending character in string.
	 * @param InString Source to search ending characters.
	 * @param Index Index of first found character.
	 * @return Whether matched character found.
	 */
	static EApplyRuleEndingCharacter FindNextEndingChar(const FString& InString, int32& Index);

protected:

	static TArray<TCHAR> ApplyRuleEndingCharacters;
};
