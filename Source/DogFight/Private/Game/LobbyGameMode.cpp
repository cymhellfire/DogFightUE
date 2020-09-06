// Dog Fight Game Code By CYM.


#include "LobbyGameMode.h"

#include "LobbyPlayerController.h"
#include "MenuModePawn.h"

ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AMenuModePawn::StaticClass();
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Request Player Info
	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(NewPlayer);
	if (PlayerController != nullptr)
	{
		if (!PlayerControllerList.Contains(PlayerController))
		{
			PlayerControllerList.Add(PlayerController);
		}

		// Listening the Player State changes
		ALobbyPlayerState* MyPlayerState = PlayerController->GetPlayerState<ALobbyPlayerState>();
		if (MyPlayerState != nullptr)
		{
			MyPlayerState->OnLobbyPlayerStateChanged.AddDynamic(this, &ALobbyGameMode::OnLobbyMemberInfoChanged);
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting);
	if (LobbyPlayerController != nullptr && PlayerControllerList.Contains(LobbyPlayerController))
	{
		// Unbind the delegate
		ALobbyPlayerState* MyPlayerState = LobbyPlayerController->GetPlayerState<ALobbyPlayerState>();
		if (MyPlayerState != nullptr)
		{
			MyPlayerState->OnLobbyPlayerStateChanged.RemoveDynamic(this, &ALobbyGameMode::OnLobbyMemberInfoChanged);
		}
		
		PlayerControllerList.Remove(LobbyPlayerController);
	}
}

void ALobbyGameMode::OnLobbyMemberInfoChanged(int32 PlayerId, const FLobbyPlayerInfo& PlayerInfo)
{
	// Notify all players in lobby
	for (ALobbyPlayerController* PlayerController : PlayerControllerList)
	{
		PlayerController->RpcPlayerInfoChanged(PlayerId, PlayerInfo);
	}
}
