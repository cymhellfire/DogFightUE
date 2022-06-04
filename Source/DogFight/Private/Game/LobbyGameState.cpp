// Dog Fight Game Code By CYM.


#include "Game/LobbyGameState.h"

#include "GameInstance/DogFightGameInstance.h"

ALobbyGameState::ALobbyGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIPlayerCount = 2;
}

void ALobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	// Also record the player state if it meets condition
	if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState))
	{
		if (!LobbyPlayerArray.Contains(LobbyPlayerState))
		{
			LobbyPlayerArray.Add(LobbyPlayerState);

			// Bind delegate on server side
			//if (GetNetMode() == NM_ListenServer)
			{
				LobbyPlayerState->OnLobbyPlayerStateChanged.AddDynamic(this, &ALobbyGameState::OnLobbyPlayerInfoChanged);
			}
		}
	}
}

void ALobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	// Also remove target from LobbyPlayerArray
	if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState))
	{
		if (LobbyPlayerArray.Contains(LobbyPlayerState))
		{
			LobbyPlayerArray.Remove(LobbyPlayerState);

			// Remove delegate on server side
			//if (GetNetMode() == NM_ListenServer)
			{
				LobbyPlayerState->OnLobbyPlayerStateChanged.RemoveDynamic(this, &ALobbyGameState::OnLobbyPlayerInfoChanged);
			}
		}
	}
	// If someone left game, it's also treated as player changed info.
	OnLobbyPlayerInfoChanged();

	Super::RemovePlayerState(PlayerState);
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	// Sync GameInstance AICount
	if (GetNetMode() != NM_Client)
	{
		if (UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
		{
			AIPlayerCount = GameInstance->GameAICount;
		}
	}
}

void ALobbyGameState::OnLobbyPlayerInfoChanged()
{
	UE_LOG(LogDogFight, Log, TEXT("OnLobbyPlayerInfoChanged"));
	
	// Notify all clients there is one player changed state
	OnLobbyPlayerStateChanged.Broadcast();
	
	// Check if game is ready to start
	const bool Ready = IsGameReady();
	if (Ready != bIsGameReady)
	{
		bIsGameReady = Ready;

		OnGameReadyChanged.Broadcast(bIsGameReady);
	}
}

void ALobbyGameState::OnRep_AIPlayerCount()
{
	OnAIPlayerCountChanged.Broadcast();
}

void ALobbyGameState::SetAIPlayerCount(int32 NewCount)
{
	AIPlayerCount = NewCount;

	if (GetNetMode() != NM_Client)
	{
		OnRep_AIPlayerCount();

		// Set to GameInstance
		if (UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
		{
			GameInstance->GameAICount = AIPlayerCount;
		}
	}
}

bool ALobbyGameState::IsGameReady()
{
	int32 ReadyCount = 0;
	for(ALobbyPlayerState* LobbyPlayerState : LobbyPlayerArray)
	{
		if (LobbyPlayerState->GetLobbyStatus() != EPlayerLobbyStatus::Preparing)
		{
			ReadyCount++;
		}
	}

	return ReadyCount == LobbyPlayerArray.Num();
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, bIsGameReady);
	DOREPLIFETIME(ALobbyGameState, AIPlayerCount);
}

