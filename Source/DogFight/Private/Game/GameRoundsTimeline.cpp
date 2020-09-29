// Dog Fight Game Code By CYM.


#include "GameRoundsTimeline.h"

AGameRoundsTimeline::AGameRoundsTimeline(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AGameRoundsTimeline::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameRoundsTimeline, TimelinePlayerInfoList);
}

void AGameRoundsTimeline::RegisterPlayer(int32 PlayerId, FString PlayerName)
{
	for (FTimelinePlayerInfo PlayerInfo : TimelinePlayerInfoList)
	{
		// Rename the existing player
		if (PlayerInfo.PlayerId == PlayerId)
		{
			RenamePlayer(PlayerId, PlayerName);
			return;
		}
	}

	const FTimelinePlayerInfo NewPlayer{PlayerId, PlayerName, TimelinePlayerInfoList.Num(), 0};
	TimelinePlayerInfoList.Add(NewPlayer);

	UE_LOG(LogDogFight, Log, TEXT("Register player [%s] with index [%d] to Timeline."), *PlayerName, PlayerId);

	// Invoke the OnRep function on server side manually
	if (GetNetMode() != NM_Client)
	{
		OnRep_TimelinePlayerInfoList();
	}
}

void AGameRoundsTimeline::RenamePlayer(int32 PlayerId, FString NewName)
{
	for (int i = 0; i < TimelinePlayerInfoList.Num(); ++i)
	{
		if (TimelinePlayerInfoList[i].PlayerId == PlayerId)
		{
			if (TimelinePlayerInfoList[i].PlayerName != NewName)
			{
				const FTimelinePlayerInfo NewInfo{PlayerId, NewName, TimelinePlayerInfoList[i].TimelineIndex, TimelinePlayerInfoList[i].FinishedRounds}; 
				TimelinePlayerInfoList.RemoveAt(i);
				TimelinePlayerInfoList.Insert(NewInfo, i);
				UE_LOG(LogDogFight, Log, TEXT("Rename player [%d] to %s"), PlayerId, *NewName);
			}
			break;
		}
	}
}

void AGameRoundsTimeline::RemovePlayer(int32 PlayerId)
{
	int32 TargetIndex = -1;
	for(int32 i = 0; i < TimelinePlayerInfoList.Num(); ++i)
	{
		if (TimelinePlayerInfoList[i].PlayerId == PlayerId)
		{
			TargetIndex = i;
			break;
		}
	}

	if (TargetIndex != -1)
	{
		TimelinePlayerInfoList.RemoveAt(TargetIndex);

		UE_LOG(LogDogFight, Log, TEXT("Remove player with index %d from Timeline."), PlayerId);

		// Invoke the OnRep function on server side manually
		if (GetNetMode() != NM_Client)
		{
			OnRep_TimelinePlayerInfoList();
		}
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to remove player with index %d from Timeline."), PlayerId);
	}
}

void AGameRoundsTimeline::RandomizeOrder()
{
	if (TimelinePlayerInfoList.Num() < 2)
	{
		return;
	}

	TArray<int32> RoundOrder;
	for (int i = 0; i < TimelinePlayerInfoList.Num(); ++i)
	{
		RoundOrder.Add(i);
	}

	// Randomize the order with Fisher-Yates Shuffle algorithm
	// No need to check the number at 0 position
	for (int i = RoundOrder.Num() - 1; i > 0; --i)
	{
		const int32 Temp = RoundOrder[i];
		const int32 SwitchTargetIndex = FMath::RandRange(0, i);
		// Switch value
		RoundOrder[i] = RoundOrder[SwitchTargetIndex];
		RoundOrder[SwitchTargetIndex] = Temp;
	}

#if !UE_BUILD_SHIPPING
	FString OrderString;
#endif

	// Apply new order to PlayerInfo
	for (int i = 0 ; i < TimelinePlayerInfoList.Num(); ++i)
	{
		TimelinePlayerInfoList[i].TimelineIndex = RoundOrder[i];
#if !UE_BUILD_SHIPPING
		OrderString.Append(FString::Printf(TEXT("%d "), RoundOrder[i]));
#endif
	}

#if !UE_BUILD_SHIPPING
	UE_LOG(LogDogFight, Log, TEXT("New Timeline Order: %s"), *OrderString);
#endif
}

void AGameRoundsTimeline::SortTimelineByIndex()
{
	TimelinePlayerInfoList.Sort([](const FTimelinePlayerInfo& A, const FTimelinePlayerInfo& B) { return A.TimelineIndex < B.TimelineIndex; });

	// Invoke OnRep on server side
	if (GetNetMode() != NM_Client)
	{
		OnRep_TimelinePlayerInfoList();
	}
}

void AGameRoundsTimeline::OnRep_TimelinePlayerInfoList()
{
	// Broadcast the event
	OnTimelinePlayerInfoListChanged.Broadcast();
}
