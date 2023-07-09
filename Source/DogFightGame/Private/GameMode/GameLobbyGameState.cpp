// Dog Fight Game Code By CYM.


#include "GameMode/GameLobbyGameState.h"

#include "Common/LuaEventDef.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Player/GameLobbyPlayerState.h"
#include "DataAsset/GameplayExperience.h"

// Sets default values
AGameLobbyGameState::AGameLobbyGameState()
{
	
}

void AGameLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AGameLobbyGameState, bIsGameReady, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AGameLobbyGameState, AIPlayerCount, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AGameLobbyGameState, GameplayExperience, SharedParams);
}

void AGameLobbyGameState::OnRep_IsGameReady()
{
	// Trigger lua event
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_OnGameReadyChanged, bIsGameReady)
}

void AGameLobbyGameState::OnRep_AIPlayerCount()
{
	// Trigger lua event
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_OnAIPlayerCountChanged, AIPlayerCount)
}

void AGameLobbyGameState::OnRep_GameplayExperience()
{
	// Trigger lua event
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_OnGameplayExperienceChanged, GameplayExperience);
}

void AGameLobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (auto PS = Cast<AGameLobbyPlayerState>(PlayerState))
	{
		LobbyPlayerStateList.AddUnique(PS);

		PS->OnReadyStatusChanged.AddUniqueDynamic(this, &AGameLobbyGameState::OnPlayerReadyStatusChanged);
	}

	// Trigger lua event
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_OnPlayerEnterLobby)
}

void AGameLobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	if (auto PS = Cast<AGameLobbyPlayerState>(PlayerState))
	{
		LobbyPlayerStateList.Remove(PS);

		PS->OnReadyStatusChanged.RemoveDynamic(this, &AGameLobbyGameState::OnPlayerReadyStatusChanged);
	}

	// Trigger lua event
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_OnPlayerLeaveLobby)
}

void AGameLobbyGameState::ServerSetAIPlayerCount_Implementation(int32 InCount)
{
	if (InCount == AIPlayerCount)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(AGameLobbyGameState, AIPlayerCount, this);
	AIPlayerCount = InCount;

	if (HasAuthority())
	{
		OnRep_AIPlayerCount();
	}
}

void AGameLobbyGameState::ServerSetGameplayExperience_Implementation(UGameplayExperience* InGameplayExperience)
{
	if (InGameplayExperience == GameplayExperience)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(AGameLobbyGameState, GameplayExperience, this);
	GameplayExperience = InGameplayExperience;

	if (HasAuthority())
	{
		OnRep_GameplayExperience();
	}
}

void AGameLobbyGameState::OnPlayerReadyStatusChanged(AGameLobbyPlayerState* PlayerState, bool bReady)
{
	if (HasAuthority())
	{
		UpdateGameReadyStatus();
	}
}

void AGameLobbyGameState::UpdateGameReadyStatus()
{
	bool bAllReady = true;
	// Check if all players are ready
	for (auto PS : LobbyPlayerStateList)
	{
		if (!PS->GetPlayerHostStatus() && !PS->GetPlayerReadyStatus())
		{
			bAllReady = false;
			break;
		}
	}

	// Update game ready status
	SetGameReady(bAllReady);
}

void AGameLobbyGameState::SetGameReady(bool bReady)
{
	if (bIsGameReady == bReady)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(AGameLobbyGameState, bIsGameReady, this);
	bIsGameReady = bReady;

	if (HasAuthority())
	{
		OnRep_IsGameReady();
	}
}
