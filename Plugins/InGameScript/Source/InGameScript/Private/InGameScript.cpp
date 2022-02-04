// Copyright Epic Games, Inc. All Rights Reserved.

#include "InGameScript.h"

#define LOCTEXT_NAMESPACE "FInGameScriptModule"

DEFINE_LOG_CATEGORY(LogInGameScript);

void FInGameScriptModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogInGameScript, Log, TEXT("[InGameScript] Module startup."));
}

void FInGameScriptModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogInGameScript, Log, TEXT("[InGameScript] Module shutdown."));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInGameScriptModule, InGameScript)