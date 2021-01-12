// Dog Fight Game Code By CYM.


#include "Player/LobbyPlayerController.h"

#include "DogFight.h"
#include "Game/DogFightGameInstance.h"
#include "Game/LobbyPlayerState.h"
#include "Game/SaveGameManager.h"
#include "Game/DogFightSaveGame.h"
#include "Game/LobbyGameMode.h"

void ALobbyPlayerController::GatherPlayerInfo()
{
	// Get player name
	UDogFightSaveGame* SaveGameInstance = Cast<UDogFightGameInstance>(GetGameInstance())->GetSaveGameManager()->GetCurrentSaveGameInstance();
	if (SaveGameInstance == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No available player profile."));
		return;
	}
	
	// Get Player State
	ALobbyPlayerState* MyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (MyPlayerState == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No available LobbyPlayerState."));
		return;
	}
	
	FLobbyPlayerInfo PlayerInfo;
	PlayerInfo.PlayerId = 0;		// This UniqueId is assigned by server, so this value has no meaning
	PlayerInfo.PlayerName = SaveGameInstance->PlayerName;
	PlayerInfo.PlayerStatus = GetNetMode() == NM_ListenServer ? EPlayerLobbyStatus::Host : MyPlayerState->GetLobbyStatus();

	UE_LOG(LogDogFight, Log, TEXT("Send Player Info: Name[%s] Status[%d]"), *PlayerInfo.PlayerName, PlayerInfo.PlayerStatus);
	
	// Send info to server
	ServerSendPlayerInfo(PlayerInfo);
}

void ALobbyPlayerController::ServerSendPlayerInfo_Implementation(FLobbyPlayerInfo PlayerInfo)
{
	// Change player name
	ServerChangeName(PlayerInfo.PlayerName);

	// Set the player statue
	ALobbyPlayerState* MyPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (MyPlayerState != nullptr)
	{
		MyPlayerState->SetPlayerInfo(PlayerInfo);
	}
}

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
}

void ALobbyPlayerController::ToggleReadyStatus()
{
	if (ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>())
	{
		LobbyPlayerState->SetLobbyStatus(LobbyPlayerState->GetLobbyStatus() == EPlayerLobbyStatus::Preparing ?
			EPlayerLobbyStatus::Ready : EPlayerLobbyStatus::Preparing);
	}
}
