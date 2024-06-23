// Dog Fight Game Code By CYM.


#include "Player/GameLobbyPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


// Sets default values
AGameLobbyPlayerState::AGameLobbyPlayerState()
{
	
}

void AGameLobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AGameLobbyPlayerState, bIsReady, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AGameLobbyPlayerState, bIsHost, SharedParams);
}

void AGameLobbyPlayerState::ServerSetPlayerReadyStatus_Implementation(bool bReady)
{
	if (bIsReady == bReady)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(AGameLobbyPlayerState, bIsReady, this);
	bIsReady = bReady;

	// Trigger event on server side as well
	if (HasAuthority())
	{
		OnRep_IsReady();
	}
}

void AGameLobbyPlayerState::ServerSetPlayerHostStatus_Implementation(bool bHost)
{
	if (bIsHost == bHost)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(AGameLobbyPlayerState, bIsHost, this);
	bIsHost = bHost;

	// Trigger event on server side as well
	if (HasAuthority())
	{
		OnRep_IsHost();
	}
}

void AGameLobbyPlayerState::OnRep_IsReady()
{
	OnReadyStatusChanged.Broadcast(this, bIsReady);
}

void AGameLobbyPlayerState::OnRep_IsHost()
{
	OnHostStatusChanged.Broadcast(this, bIsHost);
}

void AGameLobbyPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	OnPlayerNameChanged.Broadcast(this, GetPlayerName());
}
