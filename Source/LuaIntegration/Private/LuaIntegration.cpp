#include "LuaIntegration.h"

#define LOCTEXT_NAMESPACE "FLuaIntegrationModule"

void FLuaIntegrationModule::StartupModule()
{
}

void FLuaIntegrationModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLuaIntegrationModule, LuaIntegration)