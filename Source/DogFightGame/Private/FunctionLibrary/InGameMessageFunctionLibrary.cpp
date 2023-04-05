#include "FunctionLibrary/InGameMessageFunctionLibrary.h"

#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "PlayerController/TopDownStylePlayerController.h"

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

void UInGameMessageFunctionLibrary::SendInGameChatMessage(const FInGameChatMessage& ChatMessage)
{
	if (auto PC = UCommonGameFlowFunctionLibrary::GetLocalPlayerController())
	{
		PC->ServerSendInGameChatMessage(ChatMessage);
	}
}

UInGameMessageReceiverComponent* UInGameMessageFunctionLibrary::GetLocalPlayerMessageReceiver()
{
	if (auto PC = UCommonGameFlowFunctionLibrary::GetLocalPlayerController())
	{
		if (auto InGameMessageReceiver = PC->GetInGameMessageReceiverComponent())
		{
			return InGameMessageReceiver;
		}
	}

	return nullptr;
}
