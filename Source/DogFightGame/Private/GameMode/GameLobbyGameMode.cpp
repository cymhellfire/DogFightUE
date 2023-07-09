// Dog Fight Game Code By CYM.


#include "GameMode/GameLobbyGameMode.h"

#include "GameMode/GameLobbyGameState.h"
#include "Player/GameLobbyPlayerState.h"
#include "PlayerController/GameLobbyPlayerController.h"


// Sets default values
AGameLobbyGameMode::AGameLobbyGameMode()
{
	GameStateClass = AGameLobbyGameState::StaticClass();
	PlayerControllerClass = AGameLobbyPlayerController::StaticClass();
	PlayerStateClass = AGameLobbyPlayerState::StaticClass();
}

