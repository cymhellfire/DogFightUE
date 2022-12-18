#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"
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
		LuaEventService->SendEventToLua_OneParam(ELuaEvent::Type::LuaEvent_ReceiveTitleMessage, Message.ToString());
	}
}
