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
	DOREPLIFETIME_WITH_PARAMS_FAST(AGameLobbyPlayerState, LobbyPlayerInfo, SharedParams);
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

void AGameLobbyPlayerState::ServerUpdatePlayerInfo_Implementation(const FGameLobbyPlayerInfo& PlayerInfo)
{
	if (LobbyPlayerInfo == PlayerInfo)
		return;

	auto OldData = LobbyPlayerInfo;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGameLobbyPlayerState, LobbyPlayerInfo, this);
	LobbyPlayerInfo = PlayerInfo;

	// Trigger event on server side as well
	if (HasAuthority())
	{
		OnRep_LobbyPlayerInfo(OldData);
	}
}

void AGameLobbyPlayerState::OnRep_IsReady()
{
	OnReadyStatusChanged.Broadcast(this, bIsReady);
}

#define DIFF_INFO(Member)	\
	(OldData.Member != LobbyPlayerInfo.Member)

void AGameLobbyPlayerState::OnRep_LobbyPlayerInfo(const FGameLobbyPlayerInfo& OldData)
{
	OnPlayerLobbyInfoChanged.Broadcast(this, LobbyPlayerInfo);

	// Broadcast events
	if (DIFF_INFO(bHost))
	{
		OnHostStatusChanged.Broadcast(this, LobbyPlayerInfo.bHost);
	}
	if (DIFF_INFO(AvatarId))
	{
		OnAvatarIdChanged.Broadcast(this, LobbyPlayerInfo.AvatarId);
	}
}

void AGameLobbyPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	OnPlayerNameChanged.Broadcast(this, GetPlayerName());
}
