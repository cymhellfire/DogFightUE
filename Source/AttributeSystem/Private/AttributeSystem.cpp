#include "AttributeSystem.h"

#define LOCTEXT_NAMESPACE "FAttributeSystemModule"

DEFINE_LOG_CATEGORY(LogAttributeSystem)

void FAttributeSystemModule::StartupModule()
{
}

void FAttributeSystemModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FAttributeSystemModule, AttributeSystem)