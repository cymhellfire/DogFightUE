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
	ReservedMap.Add("number", EReservedType::RT_Number);
	ReservedMap.Add("string", EReservedType::RT_String);
	ReservedMap.Add("bool", EReservedType::RT_Boolean);
	ReservedMap.Add("void", EReservedType::RT_Void);
	ReservedMap.Add("return", EReservedType::RT_Return);
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

	int32 StartPos = OwningReader->GetCharNum();
	FString CurrentLine = OwningReader->GetCurrentLine();
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
	case '.': case '(': case ')': case '[':
	case ']': case '{': case '}': case ';': case ',':
		{
			ESingleSymbolType::Type OutType;
			if (ReadSingleSymbol(OutType))
			{
				// Create single symbol token
				Result = MakeShareable(new FSingleSymbolToken(OutType));
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
					Result = MakeShareable(new FIDToken(FName(IdValue)));
				}
			}
		}
		else if (IsUnaryOperator(CurChar))
		{
			// Check if this is a negative number
			if (IsDigit(OwningReader->GetNextChar()))
			{
				float NumberValue = 0.f;
				if (ReadNumber(NumberValue))
				{
					// Create a value token
					Result = MakeShareable(new FValueToken(NumberValue));
				}
			}
			else
			{
				EUnaryOperatorType::Type UnaryType;
				if (ReadUnaryOperator(UnaryType))
				{
					// Create a unary operator
					Result = MakeShareable(new FOperatorToken(UnaryType));
				}
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

	if (Result)
	{
		int32 EndPos = OwningReader->GetCharNum();
		Result->LineNum = OwningReader->GetLineNum() + 1;
		Result->StartPos = StartPos + 1;
		Result->TokenLength = EndPos - StartPos;
		Result->CurrentLine = CurrentLine;
	}

	return Result;
}

TCHAR FScriptLexer::GetCurrentChar(bool bGetSkippable) const
{
	if (OwningReader.IsValid())
	{
		TCHAR CurrentChar = OwningReader->GetCurrentChar();
		while (IsSkippableChar(CurrentChar) && !bGetSkippable)
		{
			CurrentChar = OwningReader->ConsumeAndGetNextChar();
		}
		return CurrentChar;
	}
	return INVALID_CHAR;
}

void FScriptLexer::ConsumeCurrentChar()
{
	if (OwningReader.IsValid())
	{
		OwningReader->ConsumeChar();
	}
}

TCHAR FScriptLexer::GetNextChar() const
{
	return OwningReader.IsValid() ? OwningReader->GetNextChar() : INVALID_CHAR;
}

bool FScriptLexer::GetCurrentCharPosition(int32& LineNum, int32& CharNum) const
{
	if (!OwningReader.IsValid())
		return false;

	LineNum = OwningReader->GetLineNum() + 1;
	CharNum = OwningReader->GetCharNum() + 1;
	return !OwningReader->IsFinished();
}

bool FScriptLexer::ReadNumber(float& OutValue)
{
	bool bRead = false;
	bool bNegative = false;
	OutValue = 0;
	// Check negative operator first
	TCHAR CurChar = OwningReader->GetCurrentChar();
	if (CurChar == '-')
	{
		bNegative = true;
		CurChar = OwningReader->ConsumeAndGetNextChar();
	}

	// Handle integer part
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

	// Apply the negative operator
	if (bNegative)
	{
		OutValue *= -1;
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

bool FScriptLexer::ReadSingleSymbol(ESingleSymbolType::Type& OutType)
{
	bool bRead = true;
	TCHAR CurChar = OwningReader->GetAndConsumeCurrentChar();
	switch(CurChar)
	{
	case '.':
		OutType = ESingleSymbolType::SST_Period;
		break;
	case '(':
		OutType = ESingleSymbolType::SST_OpenParen;
		break;
	case ')':
		OutType = ESingleSymbolType::SST_CloseParen;
		break;
	case '[':
		OutType = ESingleSymbolType::SST_OpenBracket;
		break;
	case ']':
		OutType = ESingleSymbolType::SST_CloseBracket;
		break;
	case '{':
		OutType = ESingleSymbolType::SST_OpenCurly;
		break;
	case '}':
		OutType = ESingleSymbolType::SST_CloseCurly;
		break;
	case ';':
		OutType = ESingleSymbolType::SST_Semicolon;
		break;
	case ',':
		OutType = ESingleSymbolType::SST_Comma;
		break;
	default:
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptLexer] Invalid single symbol %c"), CurChar);
		bRead = false;
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
