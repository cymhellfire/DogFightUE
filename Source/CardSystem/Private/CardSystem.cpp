#include "CardSystem.h"

#define LOCTEXT_NAMESPACE "FCardSystemModule"

DEFINE_LOG_CATEGORY(LogCardSystem)

void FCardSystemModule::StartupModule()
{
	UE_LOG(LogCardSystem, Log, TEXT("Module startup."));
}

void FCardSystemModule::ShutdownModule()
{
	UE_LOG(LogCardSystem, Log, TEXT("Module shutdown."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_GAME_MODULE(FCardSystemModule, CardSystem)