#include "UnrealTinyXml.h"

#define LOCTEXT_NAMESPACE "FUnrealTinyXml"

DEFINE_LOG_CATEGORY(LogUnrealTinyXml)

void FUnrealTinyXmlModule::StartupModule()
{
	UE_LOG(LogUnrealTinyXml, Log, TEXT("Module startup."));
}

void FUnrealTinyXmlModule::ShutdownModule()
{
	UE_LOG(LogUnrealTinyXml, Log, TEXT("Module shutdown."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_GAME_MODULE(FUnrealTinyXmlModule, UnrealTinyXml)
