// Dog Fight Game Code By CYM.


#include "Subsystem/InGameStatistics/InGameStatisticsSubsystem.h"

#include "Common/DogFightGameLog.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/DogFightGameModeBase.h"
#include "Player/GameLobbyPlayerState.h"

void UInGameStatisticsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FGameModeEvents::OnGameModePostLoginEvent().AddUObject(this, &UInGameStatisticsSubsystem::OnPlayerLogin);
}

void UInGameStatisticsSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FGameModeEvents::OnGameModePostLoginEvent().RemoveAll(this);
}

void UInGameStatisticsSubsystem::RegisterPlayer(APlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState))
	{
		return;
	}

	// Prevent duplicated items
	auto& NetId = InPlayerState->GetUniqueId();
	auto NetIdStr = NetId.ToString();
	FInGameStatisticsPlayerInfo* PlayerInfo;

	if (PlayerStatisticsMap.Contains(NetIdStr))
	{
		PlayerInfo = PlayerStatisticsMap.Find(NetIdStr);
		DFLogW(LogDogFightGame, TEXT("Reuse statistics with net id: %s"), *NetIdStr);
	}
	else
	{
		PlayerInfo = &PlayerStatisticsMap.Add(NetIdStr);
		DFLog(LogDogFightGame, TEXT("Add statistics with net id: %s"), *NetIdStr)
	}

	// Initialize player info
	if (PlayerInfo)
	{
		PlayerInfo->GameLobbyPlayerInfo.PlayerName = InPlayerState->GetPlayerName();
	}

	// Listen events
	if (auto LobbyState = Cast<AGameLobbyPlayerState>(InPlayerState))
	{
		RegisterGameLobbyPlayerState(LobbyState);
	}
}

void UInGameStatisticsSubsystem::UnregisterPlayer(APlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState))
	{
		return;
	}

	auto& NetId = InPlayerState->GetUniqueId();
	PlayerStatisticsMap.Remove(NetId.ToString());
	DFLog(LogDogFightGame, TEXT("Remove statistics of player: %s"), *InPlayerState->GetPlayerName());
}

void UInGameStatisticsSubsystem::ClearAllStatistics()
{
	DFLog(LogDogFightGame, TEXT("Clear all statistics."));
	PlayerStatisticsMap.Empty();
}

FInGameStatisticsPlayerInfo const* UInGameStatisticsSubsystem::GetStatisticsByPlayerState(
	const APlayerState* InPlayerState) const
{
	if (IsValid(InPlayerState))
	{
		const auto& NetId = InPlayerState->GetUniqueId();
		return PlayerStatisticsMap.Find(NetId.ToString());
	}
	return nullptr;
}

FInGameStatisticsPlayerInfo* UInGameStatisticsSubsystem::GetMutableStatisticsByPlayerState(const APlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState))
	{
		DFLogW(LogDogFightGame, TEXT("Cannot find statistic by null player state."));
		return nullptr;
	}

	auto& NetId = InPlayerState->GetUniqueId();
	return PlayerStatisticsMap.Find(NetId.ToString());
}

void UInGameStatisticsSubsystem::OnPlayerLogin(AGameModeBase* GameMode, APlayerController* PC)
{
	if (auto DFGameMode = Cast<ADogFightGameModeBase>(GameMode))
	{
		if (auto PS = PC->GetPlayerState<APlayerState>())
		{
			RegisterPlayer(PS);
		}
	}
}

void UInGameStatisticsSubsystem::RegisterGameLobbyPlayerState(AGameLobbyPlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState))
		return;

	DFLog(LogDogFightGame, TEXT("Register game lobby player state: %s"), *InPlayerState->GetUniqueNetIdStr());
	// Listening info update events
	InPlayerState->OnEndPlay.AddUniqueDynamic(this, &UInGameStatisticsSubsystem::OnPlayerStateEndPlay);
	InPlayerState->OnPlayerLobbyInfoChanged.AddUObject(this, &UInGameStatisticsSubsystem::OnPlayerLobbyInfoChanged);
}

void UInGameStatisticsSubsystem::UnregisterGameLobbyPlayerState(AGameLobbyPlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState))
		return;

	DFLog(LogDogFightGame, TEXT("Unregister game lobby player state: %s"), *InPlayerState->GetUniqueNetIdStr());
	// Remove info update listening
	InPlayerState->OnEndPlay.RemoveAll(this);
	InPlayerState->OnPlayerLobbyInfoChanged.RemoveAll(this);
}

void UInGameStatisticsSubsystem::OnPlayerStateEndPlay(AActor* InActor, EEndPlayReason::Type InReason)
{
	// Do unregister based on type
	if (auto LobbyPlayerState = Cast<AGameLobbyPlayerState>(InActor))
	{
		UnregisterGameLobbyPlayerState(LobbyPlayerState);
	}
}

void UInGameStatisticsSubsystem::OnPlayerLobbyInfoChanged(AGameLobbyPlayerState* InPlayerState, const FGameLobbyPlayerInfo& InInfo)
{
	if (!IsValid(InPlayerState))
	{
		return;
	}

	if (auto Statistics = GetMutableStatisticsByPlayerState(InPlayerState))
	{
		Statistics->GameLobbyPlayerInfo = InInfo;
	}
}

