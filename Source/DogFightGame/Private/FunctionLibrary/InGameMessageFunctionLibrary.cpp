#include "FunctionLibrary/InGameMessageFunctionLibrary.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"

void UInGameMessageFunctionLibrary::SetTitleMessage(const FText& InText)
{
	auto CurWorld = ULuaIntegrationFunctionLibrary::GetCurrentWorld();
	if (CurWorld == nullptr)
	{
		return;
	}

	if (auto GameMode = Cast<ATopDownStyleGameMode>(CurWorld->GetAuthGameMode()))
	{
		if (auto InGameMessageSender = GameMode->GetInGameMessageSender())
		{
			InGameMessageSender->BroadcastTitleMessage(InText);
		}
	}
}
