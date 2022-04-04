#pragma once
#include "InGameScript.h"
#include "TokenDefine.h"
#include "IO/ScriptReader.h"

class INGAMESCRIPT_API FScriptLexer
{
public:
	friend class FScriptLexerFactory;

	TSharedPtr<FTokenBase> GetNextToken();

	TCHAR GetCurrentChar(bool bGetSkippable = false) const;
	void ConsumeCurrentChar();
	TCHAR GetNextChar() const;
	bool GetCurrentCharPosition(int32& LineNum, int32& CharNum) const;

	static void LogWithToken(TSharedPtr<FTokenBase> InToken, FString LogContent, ELogVerbosity::Type InLogLevel)
	{
		switch(InLogLevel)
		{
		case ELogVerbosity::Warning:
			WarningWithToken(InToken, LogContent);
			break;
		case ELogVerbosity::Error:
			ErrorWithToken(InToken, LogContent);
			break;
		case ELogVerbosity::Log:
		default:
			LogWithToken(InToken, LogContent);
		}
	}

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

	bool ReadSingleSymbol(ESingleSymbolType::Type& OutType);

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

	static void LogWithToken(TSharedPtr<FTokenBase> InToken, FString LogContent)
	{
		if (InToken)
		{
			int32 LineNum = InToken->LineNum;
			int32 StartPos = InToken->StartPos;
			UE_LOG(LogInGameScript, Log, TEXT("%s [%d, %d]"), *LogContent, LineNum, StartPos);
			UE_LOG(LogInGameScript, Log, TEXT("%s"), *InToken->CurrentLine);
			FString LocationString;
			for (int32 i = 1; i < StartPos; ++i)
			{
				LocationString.AppendChar(' ');
			}
			for (int32 i = 0; i < InToken->TokenLength; ++i)
			{
				LocationString.AppendChar('^');
			}
			UE_LOG(LogInGameScript, Log, TEXT("%s"), *LocationString);
		}
	}

	static void WarningWithToken(TSharedPtr<FTokenBase> InToken, FString LogContent)
	{
		if (InToken)
		{
			int32 LineNum = InToken->LineNum;
			int32 StartPos = InToken->StartPos;
			UE_LOG(LogInGameScript, Warning, TEXT("%s [%d, %d]"), *LogContent, LineNum, StartPos);
			UE_LOG(LogInGameScript, Warning, TEXT("%s"), *InToken->CurrentLine);
			FString LocationString;
			for (int32 i = 1; i < StartPos; ++i)
			{
				LocationString.AppendChar(' ');
			}
			for (int32 i = 0; i < InToken->TokenLength; ++i)
			{
				LocationString.AppendChar('^');
			}
			UE_LOG(LogInGameScript, Warning, TEXT("%s"), *LocationString);
		}
	}

	static void ErrorWithToken(TSharedPtr<FTokenBase> InToken, FString LogContent)
	{
		if (InToken)
		{
			int32 LineNum = InToken->LineNum;
			int32 StartPos = InToken->StartPos;
			UE_LOG(LogInGameScript, Error, TEXT("%s [%d, %d]"), *LogContent, LineNum, StartPos);
			UE_LOG(LogInGameScript, Error, TEXT("%s"), *InToken->CurrentLine);
			FString LocationString;
			for (int32 i = 1; i < StartPos; ++i)
			{
				LocationString.AppendChar(' ');
			}
			for (int32 i = 0; i < InToken->TokenLength; ++i)
			{
				LocationString.AppendChar('~');
			}
			UE_LOG(LogInGameScript, Error, TEXT("%s"), *LocationString);
		}
	}

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
