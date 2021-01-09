// Dog Fight Game Code By CYM.


#include "LobbyPlayerState.h"

void ALobbyPlayerState::SetPlayerInfo(FLobbyPlayerInfo& PlayerState)
{
	PlayerLobbyStatus = PlayerState.PlayerStatus;

	// Trigger the delegate
	OnLobbyPlayerStateChanged.Broadcast();
}

void ALobbyPlayerState::SetLobbyStatus(EPlayerLobbyStatus NewStatus)
{
	if (GetNetMode() == NM_Client || GetNetMode() == NM_ListenServer)
	{
		ServerSetLobbyStatus(NewStatus);
	}
}

void ALobbyPlayerState::OnRep_PlayerLobbyStatus()
{
	// Trigger the delegate
	OnLobbyPlayerStateChanged.Broadcast();
}

void ALobbyPlayerState::ServerSetLobbyStatus_Implementation(EPlayerLobbyStatus NewStatus)
{
	if (NewStatus == PlayerLobbyStatus)
	{
		return;
	}

	// Set the new status
	PlayerLobbyStatus = NewStatus;

	// Trigger the delegate
	OnLobbyPlayerStateChanged.Broadcast();
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerLobbyStatus);
}
