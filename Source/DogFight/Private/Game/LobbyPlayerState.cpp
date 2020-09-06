// Dog Fight Game Code By CYM.


#include "LobbyPlayerState.h"

void ALobbyPlayerState::SetPlayerInfo(FLobbyPlayerInfo& PlayerState)
{
	PlayerLobbyStatus = PlayerState.PlayerStatus;

	// Trigger the delegate
	OnLobbyPlayerStateChanged.Broadcast(GetPlayerId(), PlayerState);
}
