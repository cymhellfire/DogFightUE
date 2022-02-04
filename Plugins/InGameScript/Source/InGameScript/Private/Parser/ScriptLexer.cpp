#include "Parser/ScriptLexer.h"

#include "InGameScript.h"
#include "IO/ScriptReader.h"

#define CHECK_WITH_FUNC(FuncName, InChar)			FuncName(*TCHAR_TO_ANSI(&InChar))

FScriptLexer::FScriptLexer()
{
}

FScriptLexer::FScriptLexer(FString InFilePath)
	: FilePath(InFilePath)
{
	CreateReader();
}

void FScriptLexer::CreateReader()
{
	if (FilePath.IsEmpty())
	{
		return;
	}

	OwningReader = FScriptReaderFactory::LoadScriptFile(FilePath);
	InitializeParser();
}

void FScriptLexer::InitializeParser()
{
	// Initialize the reserved token map
	ReservedMap.Add("if", EReservedType::RT_If);
	ReservedMap.Add("else", EReservedType::RT_Else);
	ReservedMap.Add("for", EReservedType::RT_For);
	ReservedMap.Add("while", EReservedType::RT_While);
	ReservedMap.Add("break", EReservedType::RT_Break);
	ReservedMap.Add("continue", EReservedType::RT_Continue);
	ReservedMap.Add("class", EReservedType::RT_Class);
	ReservedMap.Add("function", EReservedType::RT_Function);
}

TSharedPtr<FTokenBase> FScriptLexer::GetNextToken()
{
	TSharedPtr<FTokenBase> Result = nullptr;
	TCHAR CurChar = OwningReader->GetCurrentChar();

	// Skip unnecessary characters
	while (IsSkippableChar(CurChar))
	{
		OwningReader->ConsumeChar();
		CurChar = OwningReader->GetCurrentChar();
	}

	switch(CurChar)
	{
	case '\"':
		{
			FString StringValue;
			if (ReadString(StringValue))
			{
				// Create a value token with string value
				Result = MakeShareable(new FValueToken(StringValue));
			}
		}
		break;
	default:
		if (IsDigit(CurChar))
		{
			float NumberValue = 0.f;
			if (ReadNumber(NumberValue))
			{
				// Create a value token with float value
				Result = MakeShareable(new FValueToken(NumberValue));
			}
		}
		else if (IsAlphabet(CurChar))
		{
			FString IdValue;
			if (ReadIdentifier(IdValue))
			{
				// Check if this is a reserved token
				EReservedType::Type ReservedType;
				if (IsReservedToken(IdValue, ReservedType))
				{
					Result = MakeShareable(new FReservedToken(ReservedType));
				}
				else
				{
					// Create a id token with parsed value
					Result = MakeShareable(new FIDToken(IdValue));
				}
			}
		}
		else if (IsUnaryOperator(CurChar))
		{
			EUnaryOperatorType::Type UnaryType;
			if (ReadUnaryOperator(UnaryType))
			{
				// Create a unary operator
				Result = MakeShareable(new FOperatorToken(UnaryType));
			}
		}
		else if (IsBinaryOperator(CurChar))
		{
			EBinaryOperatorType::Type BinaryType;
			if (ReadBinaryOperator(BinaryType))
			{
				// Create a binary operator
				Result = MakeShareable(new FOperatorToken(BinaryType));
			}
		}
		break;
	}

	// Check if no token returns and file is not finished yet
	if (Result == nullptr && !OwningReader->IsFinished())
	{
		// This situation means the file contains contents that cannot be parsed
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptLexer] Invalid token detected [%c] in file [%s] at position [%d:%d]"),
			OwningReader->GetCurrentChar(), *FilePath, OwningReader->GetLineNum() + 1, OwningReader->GetCharNum() + 1);
	}

	return Result;
}

bool FScriptLexer::ReadNumber(float& OutValue)
{
	bool bRead = false;
	OutValue = 0;
	// Handle integer part
	TCHAR CurChar = OwningReader->GetCurrentChar();
	while(IsDigit(CurChar))
	{
		bRead = true;
		OutValue = OutValue * 10.f + ToDigit(CurChar);

		// Go to next char
		CurChar = OwningReader->ConsumeAndGetNextChar();
	}

	// Handle decimal part
	if (CurChar == '.')
	{
		// Consume the point
		OwningReader->ConsumeChar();
		CurChar = OwningReader->GetCurrentChar();
		float Offset = 0.1f;
		while (IsDigit(CurChar))
		{
			OutValue += Offset * ToDigit(CurChar);
			CurChar = OwningReader->ConsumeAndGetNextChar();

			// Move the offset toward right
			Offset *= 0.1f;
		}
	}
	return bRead;
}

bool FScriptLexer::ReadString(FString& OutString)
{
	// Consume the start double quotes
	OwningReader->ConsumeChar();

	bool bRead = true;
	OutString = "";
	TCHAR CurChar = OwningReader->GetCurrentChar();
	bool bLoop = true;
	while(bLoop)
	{
		switch (CurChar)
		{
		case EOF: case '\r': case '\n':
			// This means the string is not finished
			bRead = false;
			bLoop = false;
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptLexer] Unfinished string."));
			break;
		case '\"':
			// Terminate string with char '"'
			bLoop = false;
			// Consume the ending double quotes
			//OwningReader->ConsumeChar();
			break;
		default:
			// Append new char to the end
			OutString.AppendChar(CurChar);
			break;
		}

		// Go to next char
		OwningReader->ConsumeChar();
		CurChar = OwningReader->GetCurrentChar();
	}

	return bRead;
}

bool FScriptLexer::ReadIdentifier(FString& OutString)
{
	// Identifier should start with a letter
	bool bRead = false;
	TCHAR FirstChar = OwningReader->GetCurrentChar();
	if (!IsAlphabet(FirstChar))
	{
		// Stop reading if first char is not letter
		return bRead;
	}

	bRead = true;
	OutString.Empty();
	OutString.AppendChar(FirstChar);
	OwningReader->ConsumeChar();
	TCHAR NextChar = OwningReader->GetCurrentChar();
	// There should be only letters and digits in identifier
	while(IsIdentifierAcceptable(NextChar))
	{
		OutString.AppendChar(NextChar);
		OwningReader->ConsumeChar();
		NextChar = OwningReader->GetCurrentChar();
	}
	return bRead;
}

bool FScriptLexer::ReadUnaryOperator(EUnaryOperatorType::Type& OutType)
{
	bool bRead = false;
	const TCHAR CurChar = OwningReader->GetAndConsumeCurrentChar();
	switch(CurChar)
	{
	case '-':
		OutType = EUnaryOperatorType::UOT_Negative;
		bRead = true;
		break;
	case '!':
		OutType = EUnaryOperatorType::UOT_Not;
		bRead = true;
		break;
	default:
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptLexer] Invalid unary operator %c"), CurChar);
		break;
	}

	return bRead;
}

bool FScriptLexer::ReadBinaryOperator(EBinaryOperatorType::Type& OutType)
{
	bool bRead = false;
	const TCHAR CurChar = OwningReader->GetAndConsumeCurrentChar();
	switch (CurChar)
	{
	case '+':
		OutType = EBinaryOperatorType::BOT_Add;
		bRead = true;
		break;
	case '-':
		OutType = EBinaryOperatorType::BOT_Subtract;
		bRead = true;
		break;
	case '*':
		OutType = EBinaryOperatorType::BOT_Multiply;
		bRead = true;
		break;
	case '/':
		OutType = EBinaryOperatorType::BOT_Divide;
		bRead = true;
		break;
	case '%':
		OutType = EBinaryOperatorType::BOT_Mod;
		bRead = true;
		break;
	case '^':
		OutType = EBinaryOperatorType::BOT_Factorial;
		bRead = true;
		break;
	case '=':
		// Check next char for equal operator
		if (OwningReader->GetNextChar() == '=')
		{
			OutType = EBinaryOperatorType::BOT_Equal;
			// Also consume next char
			OwningReader->ConsumeChar();
		}
		else
		{
			OutType = EBinaryOperatorType::BOT_Assign;
		}
		bRead = true;
		break;
	case '>':
		// Check next char for larger equal operator
		if (OwningReader->GetNextChar() == '=')
		{
			OutType = EBinaryOperatorType::BOT_LargerEqual;
			// Also consume next char
			OwningReader->ConsumeChar();
		}
		else
		{
			OutType = EBinaryOperatorType::BOT_Larger;
		}
		bRead = true;
		break;
	case '<':
		// Check next char for less equal operator
		if (OwningReader->GetNextChar())
		{
			OutType = EBinaryOperatorType::BOT_LessEqual;
			// Also consume next char
			OwningReader->ConsumeChar();
		}
		else
		{
			OutType = EBinaryOperatorType::BOT_Less;
		}
		bRead = true;
		break;
	default:
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptLexer] Invalid binary operator %c"), CurChar);
		break;
	}

	return bRead;
}

bool FScriptLexer::IsDigit(TCHAR InChar)
{
	const char AnsiValue = *TCHAR_TO_ANSI(&InChar);
	return IsDigit(AnsiValue);
}

bool FScriptLexer::IsDigit(char InChar)
{
	return InChar >= '0' && InChar <= '9';
}

bool FScriptLexer::IsAlphabet(TCHAR InChar)
{
	const char AnsiValue = *TCHAR_TO_ANSI(&InChar);
	return IsAlphabet(AnsiValue);
}

bool FScriptLexer::IsAlphabet(char InChar)
{
	return (InChar >= 'A' && InChar <= 'Z') || (InChar >= 'a' && InChar <= 'z');
}

bool FScriptLexer::IsUnaryOperator(TCHAR InChar)
{
	const char AnsiValue = *TCHAR_TO_ANSI(&InChar);
	return IsUnaryOperator(AnsiValue);
}

bool FScriptLexer::IsUnaryOperator(char InChar)
{
	return InChar == '-' || InChar == '!';
}

bool FScriptLexer::IsBinaryOperator(TCHAR InChar)
{
	return CHECK_WITH_FUNC(IsBinaryOperator, InChar);
}

bool FScriptLexer::IsBinaryOperator(char InChar)
{
	return InChar == '+' || InChar == '-' || InChar == '*' || InChar == '/'
		|| InChar == '%' || InChar == '^' || InChar == '=' || InChar == '>'
		|| InChar == '>' || InChar == '<';
}

bool FScriptLexer::IsSkippableChar(TCHAR InChar)
{
	return CHECK_WITH_FUNC(IsSkippableChar, InChar);
}

bool FScriptLexer::IsSkippableChar(char InChar)
{
	return InChar == ' ' || InChar == '\t' || InChar == '\n';
}

bool FScriptLexer::IsIdentifierAcceptable(TCHAR InChar) const
{
	const char AnsiValue = *TCHAR_TO_ANSI(&InChar);
	return IsDigit(AnsiValue) || IsAlphabet(AnsiValue) || AnsiValue == '_';
}

bool FScriptLexer::IsReservedToken(FString InValue, EReservedType::Type& OutType) const
{
	bool bRead = false;
	if (ReservedMap.Contains(InValue))
	{
		OutType = ReservedMap[InValue];
		bRead = true;
	}

	return bRead;
}

int32 FScriptLexer::ToDigit(TCHAR InChar) const
{
	const char AnsiValue = *TCHAR_TO_ANSI(&InChar);
	return AnsiValue - '0';
}

TSharedPtr<FScriptLexer> FScriptLexerFactory::CreateScriptLexer(FString FilePath)
{
	return MakeShareable(new FScriptLexer(FilePath));
}
