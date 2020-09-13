// Dog Fight Game Code By CYM.


#include "LobbyGameMode.h"


#include "LobbyGameState.h"
#include "LobbyPlayerController.h"
#include "MenuModePawn.h"
#include "GameFramework/GameStateBase.h"

ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AMenuModePawn::StaticClass();
	PlayerControllerClass = ALobbyPlayerController::StaticClass(); 
	PlayerStateClass = ALobbyPlayerState::StaticClass();
	GameStateClass = ALobbyGameState::StaticClass();
}

TArray<FLobbyPlayerInfo> ALobbyGameMode::GetAllPlayerInfo() const
{
	TArray<FLobbyPlayerInfo> PlayerInfos;

	TArray<APlayerState*> PlayerArray = GetGameState<AGameStateBase>()->PlayerArray;
	for (APlayerState* PlayerState : PlayerArray)
	{
		if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState))
		{
			PlayerInfos.Add(FLobbyPlayerInfo{LobbyPlayerState->GetPlayerId(), LobbyPlayerState->GetPlayerName(),
				LobbyPlayerState->GetLobbyStatus()});
		}
	}

	return PlayerInfos;
}
