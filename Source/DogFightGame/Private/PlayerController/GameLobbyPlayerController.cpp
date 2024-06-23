// Dog Fight Game Code By CYM.


#include "PlayerController/GameLobbyPlayerController.h"

#include "Common/LuaEventDef.h"
#include "FunctionLibrary/GameSettingsFunctionLibrary.h"
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
		PS->SetPlayerName(PlayerInfo.PlayerName);
		PS->ServerUpdatePlayerInfo(PlayerInfo);
	}
}

void AGameLobbyPlayerController::ServerMarkPlayerReady_Implementation(bool bIsReady)
{
	if (auto PS = GetPlayerState<AGameLobbyPlayerState>())
	{
		PS->ServerSetPlayerReadyStatus(bIsReady);
	}
}

void AGameLobbyPlayerController::ServerSelectAvatarId_Implementation(const FGameLobbyPlayerAvatarId& AvatarId)
{
	if (auto PS = GetPlayerState<AGameLobbyPlayerState>())
	{
		auto LobbyPlayerInfo = PS->GetLobbyPlayerInfo();
		LobbyPlayerInfo.AvatarId = AvatarId;
		PS->ServerUpdatePlayerInfo(LobbyPlayerInfo);
	}
}

void AGameLobbyPlayerController::GatherPlayerInfo()
{
	Super::GatherPlayerInfo();

	FGameLobbyPlayerInfo NewInfo;
	NewInfo.PlayerName = UGameSettingsFunctionLibrary::GetPlayerName();
	NewInfo.bHost = GetNetMode() != NM_Client;
	NewInfo.AvatarId = UGameSettingsFunctionLibrary::GetLastAvatarId();

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
