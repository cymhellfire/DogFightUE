#include "Utils/ScriptUtils.h"

#include "Parser/ScriptParser.h"

TSharedPtr<FScriptParser> FScriptUtils::CreateScriptParser(FString FilePath)
{
	// Check if given file path is valid
	if (FPaths::FileExists(FilePath))
	{
		return FScriptParserFactory::CreateScriptParser(FilePath);
	}

	return nullptr;
}
