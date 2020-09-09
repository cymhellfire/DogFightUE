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

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(NewPlayer);
	if (PlayerController != nullptr)
	{
		if (!PlayerControllerList.Contains(PlayerController))
		{
			PlayerControllerList.Add(PlayerController);
		}

		// Request Player Info
		PlayerController->RpcHostUploadPlayerInfo();
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting);
	if (LobbyPlayerController != nullptr && PlayerControllerList.Contains(LobbyPlayerController))
	{
		PlayerControllerList.Remove(LobbyPlayerController);
	}
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
