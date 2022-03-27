#include "IO/ScriptReader.h"
#include "Common.h"
#include "InGameScript.h"

FScriptReader::FScriptReader()
{
}

FScriptReader::FScriptReader(FString InFilePath)
	: bFinished(false)
	, FilePath(InFilePath)
{
	LoadFile();
	Initialize();
}

void FScriptReader::LoadFile()
{
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptReader] Empty file path to load."));
		return;
	}

	// Load file content
	FFileHelper::LoadFileToStringArray(FileContent, GetData(FilePath));

	if (FileContent.Num() == 0)
	{
		UE_LOG(LogInGameScript, Warning, TEXT("[ScriptReader] Script [%s] has no content."), *FilePath);
	}
}

void FScriptReader::Initialize()
{
	LineNum = 0;
	CharNum = 0;

	CurLine = FileContent[LineNum];
	NextLine = LineNum + 1 < FileContent.Num() ? FileContent[LineNum + 1] : "";
}

void FScriptReader::IncreaseLineNumber()
{
	LineNum++;
	// Also update current line content
	if (LineNum < FileContent.Num())
	{
		CurLine = FileContent[LineNum];
	}
	else
	{
		// File is end
		bFinished = true;
	}

	// Update next line if not finished
	if (!bFinished)
	{
		if (LineNum + 1 < FileContent.Num())
		{
			NextLine = FileContent[LineNum + 1];
		}
	}
}

TCHAR FScriptReader::GetCurrentChar() const
{
	// Check if reached end of file
	if (bFinished)
	{
		return INVALID_CHAR;
	}

	// Validation
	if (CharNum > CurLine.Len())
	{
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptReader] Character index [%d] out of range at line: %d"), CharNum, LineNum);
		return INVALID_CHAR;
	}

	// Return '\n' at end of line
	if (CharNum == CurLine.Len())
	{
		return '\n';
	}

	return CurLine[CharNum];
}

TCHAR FScriptReader::GetNextChar() const
{
	// Check if reached end of file
	if (bFinished)
	{
		return INVALID_CHAR;
	}

	if (CharNum >= CurLine.Len())
	{
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptReader] Character index [%d] out of range at line: %d"), CharNum, LineNum);
		return INVALID_CHAR;
	}

	// Check for next line
	if (CharNum + 1 == CurLine.Len() && NextLine != CurLine)
	{
		return NextLine[0];
	}

	return CurLine[CharNum + 1];
}

TCHAR FScriptReader::GetAndConsumeCurrentChar()
{
	const TCHAR CurChar = GetCurrentChar();
	ConsumeChar();

	return CurChar;
}

TCHAR FScriptReader::ConsumeAndGetNextChar()
{
	ConsumeChar();
	return GetCurrentChar();
}

void FScriptReader::ConsumeChar()
{
	CharNum++;

	// Check index
	if (CharNum > CurLine.Len())
	{
		CharNum = 0;
		IncreaseLineNumber();
	}
}

TSharedPtr<FScriptReader> FScriptReaderFactory::LoadScriptFile(FString FilePath)
{
	TSharedPtr<FScriptReader> Result = MakeShareable(new FScriptReader(FilePath));
	return Result;
}
