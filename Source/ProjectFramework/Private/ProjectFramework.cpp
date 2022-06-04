#include "ProjectFramework.h"

#define LOCTEXT_NAMESPACE "FProjectFrameworkModule"

DEFINE_LOG_CATEGORY(LogProjectFramework)

void FProjectFrameworkModule::StartupModule()
{
	UE_LOG(LogProjectFramework, Log, TEXT("Module startup."));
}

void FProjectFrameworkModule::ShutdownModule()
{
	UE_LOG(LogProjectFramework, Log, TEXT("Module shutdown."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_GAME_MODULE(FProjectFrameworkModule, ProjectFramework)
