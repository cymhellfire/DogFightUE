// Dog Fight Game Code By CYM.


#include "PlayerController/GameLobbyPlayerController.h"

#include "Common/LuaEventDef.h"
#include "GameService/LuaEventService.h"
#include "Player/GameLobbyPlayerState.h"


// Sets default values
AGameLobbyPlayerController::AGameLobbyPlayerController()
{
	bShowMouseCursor = true;
}

void AGameLobbyPlayerController::ServerSetPlayerInfo_Implementation(const FGameLobbyPlayerInfo& PlayerInfo)
{
	if (auto PS = GetPlayerState<AGameLobbyPlayerState>())
	{
		// PS->SetPlayerName(PlayerInfo.PlayerName);
		PS->ServerSetPlayerHostStatus(PlayerInfo.bHost);
	}
}

void AGameLobbyPlayerController::ServerMarkPlayerReady_Implementation(bool bIsReady)
{
	if (auto PS = GetPlayerState<AGameLobbyPlayerState>())
	{
		PS->ServerSetPlayerReadyStatus(bIsReady);
	}
}

void AGameLobbyPlayerController::GatherPlayerInfo()
{
	Super::GatherPlayerInfo();

	FGameLobbyPlayerInfo NewInfo;
	NewInfo.bHost = GetNetMode() != NM_Client;

	ServerSetPlayerInfo(NewInfo);
}

void AGameLobbyPlayerController::ClientReturnToMainMenuWithReason2_Implementation(EReturnToMainMenuReason::Type Reason)
{
	Super::ClientReturnToMainMenuWithReason2_Implementation(Reason);

	SEND_LUA_EVENT(ELuaEvent::LuaEvent_SessionDismiss, Reason)
}

void AGameLobbyPlayerController::ClientPreStartGame_Implementation()
{
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_NotifyGameLoading)
}

void AGameLobbyPlayerController::HandleReturnToMainMenu2()
{
	Super::HandleReturnToMainMenu2();

	SEND_LUA_EVENT(ELuaEvent::LuaEvent_SessionDismiss, EReturnToMainMenuReason::Unknown)
}
