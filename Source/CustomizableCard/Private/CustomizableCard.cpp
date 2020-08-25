#include "CustomizableCard.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FCustomizableCardModule, CustomizableCard);

DEFINE_LOG_CATEGORY(LogCustomizableCard)

void FCustomizableCardModule::StartupModule()
{
	UE_LOG(LogCustomizableCard, Log, TEXT("CustomizableCard Module started."));
}

void FCustomizableCardModule::ShutdownModule()
{
	UE_LOG(LogCustomizableCard, Log, TEXT("CustomizableCard Module shut down."));
}