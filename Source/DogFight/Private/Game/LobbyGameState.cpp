// Dog Fight Game Code By CYM.


#include "LobbyGameState.h"

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
			if (GetNetMode() == NM_ListenServer)
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
			if (GetNetMode() == NM_ListenServer)
			{
				LobbyPlayerState->OnLobbyPlayerStateChanged.RemoveDynamic(this, &ALobbyGameState::OnLobbyPlayerInfoChanged);
			}
		}
	}

	Super::RemovePlayerState(PlayerState);
}

void ALobbyGameState::OnLobbyPlayerInfoChanged(int32 PlayerId, const FLobbyPlayerInfo& PlayerInfo)
{
	// Notify all clients there is one player changed state
	RpcLobbyPlayerInfoChanged();
}

void ALobbyGameState::RpcLobbyPlayerInfoChanged_Implementation()
{
	OnLobbyPlayerStateChanged.Broadcast();
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, LobbyPlayerArray);
}

