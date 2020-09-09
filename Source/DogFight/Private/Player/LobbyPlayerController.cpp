// Dog Fight Game Code By CYM.


#include "LobbyPlayerController.h"


#include "DogFight.h"
#include "DogFightGameInstance.h"
#include "LobbyPlayerState.h"
#include "SaveGameManager.h"
#include "DogFightSaveGame.h"
#include "LobbyGameMode.h"

void ALobbyPlayerController::GatherPlayerInfo()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("GatherPlayerInfo"));

	// Get player name
	UDogFightSaveGame* SaveGameInstance = Cast<UDogFightGameInstance>(GetGameInstance())->GetSaveGameManager()->GetCurrentSaveGameInstance();
	if (SaveGameInstance == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No available player profile."));
		return;
	}

	UE_LOG(LogDogFight, Display, TEXT("PlayerState: %s"), *PlayerState->StaticClass()->GetName());

	ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
	if (LobbyPlayerState != nullptr)
	{
		UE_LOG(LogDogFight, Display, TEXT("Get Lobby Player State."));
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

	// Send info to server
	CmdSendPlayerInfo(PlayerInfo);
}

void ALobbyPlayerController::CmdSendPlayerInfo_Implementation(FLobbyPlayerInfo PlayerInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("CmdSendPlayerInfo"));
	
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

void ALobbyPlayerController::RpcPlayerStateChanged_Implementation()
{
	OnLobbyPlayerInfoChanged.Broadcast();
}

void ALobbyPlayerController::RpcHostUploadPlayerInfo_Implementation()
{
	// Gather all information and send to server (only for host)
	if (GetNetMode() == NM_ListenServer)
	{
		GatherPlayerInfo();
	}
}

void ALobbyPlayerController::RpcGameReadyStateChanged_Implementation(bool bIsReady)
{
	OnGameReadyStateChanged.Broadcast(bIsReady);
}

void ALobbyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnRep_PlayerState."));

	// Gather all information and send to server
	GatherPlayerInfo();
}
