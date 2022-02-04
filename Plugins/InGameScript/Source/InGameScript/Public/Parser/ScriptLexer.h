#pragma once
#include "TokenDefine.h"

class INGAMESCRIPT_API FScriptLexer
{
public:
	friend class FScriptLexerFactory;

	TSharedPtr<FTokenBase> GetNextToken();

protected:
	FScriptLexer();
	FScriptLexer(FString InFilePath);

	void CreateReader();

	void InitializeParser();

	bool ReadNumber(float& OutValue);

	bool ReadString(FString& OutString);

	bool ReadIdentifier(FString& OutString);

	bool ReadUnaryOperator(EUnaryOperatorType::Type& OutType);

	bool ReadBinaryOperator(EBinaryOperatorType::Type& OutType);

	static FORCEINLINE bool IsDigit(TCHAR InChar);
	static FORCEINLINE bool IsDigit(char InChar);

	static FORCEINLINE bool IsAlphabet(TCHAR InChar);
	static FORCEINLINE bool IsAlphabet(char InChar);

	static FORCEINLINE bool IsUnaryOperator(TCHAR InChar);
	static FORCEINLINE bool IsUnaryOperator(char InChar);

	static FORCEINLINE bool IsBinaryOperator(TCHAR InChar);
	static FORCEINLINE bool IsBinaryOperator(char InChar);

	static FORCEINLINE bool IsSkippableChar(TCHAR InChar);
	static FORCEINLINE bool IsSkippableChar(char InChar);

	bool IsIdentifierAcceptable(TCHAR InChar) const;

	bool IsReservedToken(FString InValue, EReservedType::Type& OutType) const;

	int32 ToDigit(TCHAR InChar) const;

protected:
	FString FilePath;

	TMap<FString, EReservedType::Type> ReservedMap;

	TSharedPtr<class FScriptReader> OwningReader;
};

class INGAMESCRIPT_API FScriptLexerFactory
{
	friend class FScriptParser;
protected:
	/**
	 * Create a script lexer for given file.
	 * @param FilePath			Script file to analysis
	 * @return Return the pointer of new script lexer
	 */
	static TSharedPtr<FScriptLexer> CreateScriptLexer(FString FilePath);
};
