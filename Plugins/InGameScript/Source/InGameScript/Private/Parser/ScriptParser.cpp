#include "Parser/ScriptParser.h"

#include "InGameScript.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Parser/ScriptLexer.h"

FScriptParser::FScriptParser()
{
}

FScriptParser::FScriptParser(FString FilePath)
{
	if (!FilePath.IsEmpty())
	{
		CreateLexer(FilePath);
	}
}

FScriptParser::~FScriptParser()
{
	UE_LOG(LogInGameScript, Log, TEXT("[ScriptParser] Instance destructed."));
}

void FScriptParser::CreateLexer(FString FilePath)
{
	OwningLexer = FScriptLexerFactory::CreateScriptLexer(FilePath);
}

bool FScriptParser::Execute()
{
	if (OwningLexer.IsValid())
	{
		// Iterate through all tokens and print info
		int32 TokenIndex = 0;
		TSharedPtr<FTokenBase> CurToken = OwningLexer->GetNextToken();
		while (CurToken.IsValid())
		{
			// Print token information
			UE_LOG(LogInGameScript, Log, TEXT("[ScriptParser] Token[%d]: %s"), TokenIndex, *CurToken->ToString());

			CurToken = OwningLexer->GetNextToken();
			TokenIndex++;
		}

		return true;
	}

	return false;
}

TSharedPtr<FScriptParser> FScriptParserFactory::CreateScriptParser(FString FilePath)
{
	return MakeShareable(new FScriptParser(FilePath));
}
