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

	static void LogWithCurrentPos(TSharedPtr<FScriptLexer> InLexer, FString LogContent, ELogVerbosity::Type InLogLevel)
	{
		switch(InLogLevel)
		{
		case ELogVerbosity::Warning:
			WarningWithCurrentPos(InLexer, LogContent);
			break;
		case ELogVerbosity::Error:
			ErrorWithCurrentPos(InLexer, LogContent);
			break;
		case ELogVerbosity::Log:
		default:
			LogWithCurrentPos(InLexer, LogContent);
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

	static void LogWithCurrentPos(TSharedPtr<FScriptLexer> InLexer, FString LogContent)
	{
		if (InLexer)
		{
			int32 LineNum, CharNum;
			if (InLexer->GetCurrentCharPosition(LineNum, CharNum))
			{
				UE_LOG(LogInGameScript, Log, TEXT("%s [%d, %d]"), *LogContent, LineNum, CharNum);
				UE_LOG(LogInGameScript, Log, TEXT("%s"), *InLexer->OwningReader->GetCurrentLine());
				FString LocationString;
				for (int32 i = 1; i < CharNum; ++i)
				{
					LocationString.AppendChar(' ');
				}
				UE_LOG(LogInGameScript, Log, TEXT("%s^"), *LocationString);
			}
			else
			{
				UE_LOG(LogInGameScript, Log, TEXT("%s"), *LogContent);
			}
		}
	}

	static void WarningWithCurrentPos(TSharedPtr<FScriptLexer> InLexer, FString LogContent)
	{
		if (InLexer)
		{
			int32 LineNum, CharNum;
			if (InLexer->GetCurrentCharPosition(LineNum, CharNum))
			{
				UE_LOG(LogInGameScript, Warning, TEXT("%s [%d, %d]"), *LogContent, LineNum, CharNum);
				UE_LOG(LogInGameScript, Warning, TEXT("%s"), *InLexer->OwningReader->GetCurrentLine());
				FString LocationString;
				for (int32 i = 1; i < CharNum; ++i)
				{
					LocationString.AppendChar(' ');
				}
				UE_LOG(LogInGameScript, Warning, TEXT("%s^"), *LocationString);
			}
			else
			{
				UE_LOG(LogInGameScript, Warning, TEXT("%s"), *LogContent);
			}
		}
	}

	static void ErrorWithCurrentPos(TSharedPtr<FScriptLexer> InLexer, FString LogContent)
	{
		if (InLexer)
		{
			int32 LineNum, CharNum;
			if (InLexer->GetCurrentCharPosition(LineNum, CharNum))
			{
				UE_LOG(LogInGameScript, Error, TEXT("%s [%d, %d]"), *LogContent, LineNum, CharNum);
				UE_LOG(LogInGameScript, Error, TEXT("%s"), *InLexer->OwningReader->GetCurrentLine());
				FString LocationString;
				for (int32 i = 1; i < CharNum; ++i)
				{
					LocationString.AppendChar(' ');
				}
				UE_LOG(LogInGameScript, Error, TEXT("%s^"), *LocationString);
			}
			else
			{
				UE_LOG(LogInGameScript, Error, TEXT("%s"), *LogContent);
			}
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
