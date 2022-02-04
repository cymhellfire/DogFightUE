#pragma once

class FScriptLexer;

class INGAMESCRIPT_API FScriptParser
{
public:
	friend class FScriptParserFactory;
	~FScriptParser();

	bool Execute();

protected:
	FScriptParser();
	FScriptParser(FString FilePath);

	void CreateLexer(FString FilePath);

protected:
	TSharedPtr<FScriptLexer> OwningLexer;
};

class INGAMESCRIPT_API FScriptParserFactory
{
public:
	static TSharedPtr<FScriptParser> CreateScriptParser(FString FilePath);
};
