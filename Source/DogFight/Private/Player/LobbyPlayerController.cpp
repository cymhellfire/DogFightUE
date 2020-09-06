// Dog Fight Game Code By CYM.


#include "LobbyPlayerController.h"


#include "DogFight.h"
#include "DogFightGameInstance.h"
#include "LobbyPlayerState.h"
#include "SaveGameManager.h"
#include "DogFightSaveGame.h"

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
	PlayerInfo.PlayerName = SaveGameInstance->PlayerName;
	PlayerInfo.PlayerStatus = MyPlayerState->GetLobbyStatus();

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

void ALobbyPlayerController::RpcPlayerInfoChanged_Implementation(int32 PlayerId, FLobbyPlayerInfo PlayerInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player(%d) changed his info."), PlayerId));

	OnLobbyPlayerInfoChanged.Broadcast(PlayerId, PlayerInfo);
}

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
}

void ALobbyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnRep_PlayerState."));

	// Gather all information and send to server
	GatherPlayerInfo();
}
