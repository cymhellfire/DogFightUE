// Dog Fight Game Code By CYM.

#include "DebugTools/DogFightCheatManager.h"
#include "DogFight.h"
#include "Parser/ScriptParser.h"
#include "Utils/ScriptUtils.h"

void UDogFightCheatManager::TestScript(FString FileName)
{
	if (FileName.IsEmpty())
	{
		return;
	}

	const FString FilePath = FString::Printf(TEXT("%sScripts/%s.igs"), *FPaths::ProjectDir(), *FileName);
	UE_LOG(LogDogFight, Log, TEXT("Test Script: %s"), *FilePath);

	TSharedPtr<FScriptParser> NewParser = FScriptUtils::CreateScriptParser(FilePath);
	if (NewParser.IsValid())
	{
		NewParser->Execute();
	}
}
