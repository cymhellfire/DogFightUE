#pragma once

class FScriptReaderFactory;

class INGAMESCRIPT_API FScriptReader
{
public:
	friend class FScriptReaderFactory;

	TCHAR GetCurrentChar() const;
	TCHAR GetNextChar() const;

	/**
	 * Get and consume current char.
	 */
	TCHAR GetAndConsumeCurrentChar();

	/**
	 * Consume current char and get next one.
	 */
	TCHAR ConsumeAndGetNextChar();

	void ConsumeChar();

	bool IsFinished() const
	{
		return bFinished;
	}

	int32 GetLineNum() const
	{
		return LineNum;
	}

	int32 GetCharNum() const
	{
		return CharNum;
	}

protected:
	FScriptReader();
	FScriptReader(FString InFilePath);

	void LoadFile();
	void Initialize();

	void IncreaseLineNumber();

protected:
	uint8 bFinished : 1;

	FString FilePath;
	FString CurLine;
	FString NextLine;
	TArray<FString> FileContent;

	int32 LineNum;
	int32 CharNum;
};

class INGAMESCRIPT_API FScriptReaderFactory
{
	friend class FScriptLexer;
protected:
	/**
	 * Load script based on given file path.
	 * @note Return instance should be maintained by invoking class.
	 * @param FilePath			Absolute file path
	 * @return New script reader instance
	 */
	static TSharedPtr<FScriptReader> LoadScriptFile(FString FilePath);
};
