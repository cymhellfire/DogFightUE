#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"

#include "Common/DogFightGameLog.h"
#include "Common/LuaEventDef.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"

UInGameMessageReceiverComponent::UInGameMessageReceiverComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UInGameMessageReceiverComponent::ReceiveMessage_Implementation(const FText& Message)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_ReceiveTitleMessage, Message.ToString());
	}
}

void UInGameMessageReceiverComponent::ReceiveInGameChatMessage_Implementation(const FInGameChatMessage& ChatMessage)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		if (CachedChatMessage.IsSet())
		{
			UE_LOG(LogDogFightGame, Warning, TEXT("[InGameMessageReceiverComponent] A cached chat message not used has been overwritten."));
		}

		CachedChatMessage = ChatMessage;
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_ReceiveInGameChatMessage);
	}
}

bool UInGameMessageReceiverComponent::GetCachedGameChatMessage(FInGameChatMessage& OutMessage)
{
	if (!CachedChatMessage.IsSet())
	{
		return false;
	}

	OutMessage = CachedChatMessage.GetValue();
	// Clear the cached message after get
	CachedChatMessage.Reset();
	return true;
}
