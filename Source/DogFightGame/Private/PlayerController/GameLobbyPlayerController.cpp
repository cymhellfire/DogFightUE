// Dog Fight Game Code By CYM.


#include "PlayerController/GameLobbyPlayerController.h"

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
