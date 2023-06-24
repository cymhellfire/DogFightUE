// Dog Fight Game Code By CYM.


#include "PlayerController/GameLobbyPlayerController.h"

#include "Player/GameLobbyPlayerState.h"


// Sets default values
AGameLobbyPlayerController::AGameLobbyPlayerController()
{
	bShowMouseCursor = true;
}

void AGameLobbyPlayerController::ServerMarkPlayerReady_Implementation(bool bIsReady)
{
	if (auto PS = GetPlayerState<AGameLobbyPlayerState>())
	{
		PS->ServerSetPlayerReadyStatus(bIsReady);
	}
}
