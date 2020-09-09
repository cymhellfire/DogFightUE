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

void ALobbyGameMode::BeginDestroy()
{
	Super::BeginDestroy();

	if (ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>())
	{
		LobbyGameState->OnLobbyPlayerStateChanged.RemoveDynamic(this, &ALobbyGameMode::OnPlayerStateChanged);
		LobbyGameState->OnGameReadyChanged.RemoveDynamic(this, &ALobbyGameMode::OnGameReadyChanged);
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>())
	{
		LobbyGameState->OnLobbyPlayerStateChanged.AddDynamic(this, &ALobbyGameMode::OnPlayerStateChanged);
		LobbyGameState->OnGameReadyChanged.AddDynamic(this, &ALobbyGameMode::OnGameReadyChanged);
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to get LobbyGameState."));
	}
}

void ALobbyGameMode::OnPlayerStateChanged()
{
	for (ALobbyPlayerController* PlayerController : PlayerControllerList)
	{
		PlayerController->RpcPlayerStateChanged();
	}
}

void ALobbyGameMode::OnGameReadyChanged(bool NewReadyState)
{
	for (ALobbyPlayerController* PlayerController : PlayerControllerList)
	{
		PlayerController->RpcGameReadyStateChanged(NewReadyState);
	}
}
