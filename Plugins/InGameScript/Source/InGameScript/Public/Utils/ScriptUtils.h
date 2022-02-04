#pragma once

class FScriptParser;

class INGAMESCRIPT_API FScriptUtils
{
public:
	static TSharedPtr<FScriptParser> CreateScriptParser(FString FilePath);
};
