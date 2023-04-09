#include "FunctionLibrary/InGameMessageFunctionLibrary.h"

#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "PlayerController/TopDownStylePlayerController.h"

void UInGameMessageFunctionLibrary::SetTitleMessage(const UObject* WorldContextObject, const FText& InText)
{
	auto CurWorld = ULuaIntegrationFunctionLibrary::GetCurrentWorld(WorldContextObject);
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

void UInGameMessageFunctionLibrary::SendInGameChatMessageAsPlayer(const UObject* WorldContextObject, const FInGameChatSendOption& SendOption)
{
	if (auto PC = UCommonGameFlowFunctionLibrary::GetLocalPlayerController(WorldContextObject))
	{
		FInGameChatMessage NewMessage;
		NewMessage.SourcePlayerId = SendOption.SourcePlayerId;
		NewMessage.SourceType = EInGameChatSourceType::Player;
		NewMessage.Content = SendOption.Content;
		PC->ServerSendInGameChatMessage(NewMessage);
	}
}

void UInGameMessageFunctionLibrary::SendInGameChatMessageAsSystem(const UObject* WorldContextObject, const FInGameChatSendOption& SendOption)
{
	if (auto GM = UCommonGameFlowFunctionLibrary::GetCurrentTopDownStyleGameMode(WorldContextObject))
	{
		if (auto Sender = GM->GetInGameMessageSender())
		{
			FInGameChatMessage NewMessage;
			NewMessage.Content = SendOption.Content;
			NewMessage.SourcePlayerId = EInGameChatSourceType::System;
			Sender->BroadcastInGameChatMessage(NewMessage);
		}
	}
}

UInGameMessageReceiverComponent* UInGameMessageFunctionLibrary::GetLocalPlayerMessageReceiver(const UObject* WorldContextObject)
{
	if (auto PC = UCommonGameFlowFunctionLibrary::GetLocalPlayerController(WorldContextObject))
	{
		if (auto InGameMessageReceiver = PC->GetInGameMessageReceiverComponent())
		{
			return InGameMessageReceiver;
		}
	}

	return nullptr;
}
