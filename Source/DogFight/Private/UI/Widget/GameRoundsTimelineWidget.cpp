// Dog Fight Game Code By CYM.


#include "GameRoundsTimelineWidget.h"

#include "StandardGameState.h"
#include "GameRoundsTimeline.h"

void UGameRoundsTimelineWidget::SetupTimelineDisplay()
{
	if (AStandardGameState* StandardGameState = GetWorld()->GetGameState<AStandardGameState>())
	{
		SynchronizePlayerInfoList();

		// Call Blueprint function
		OnDisplayPlayerInfoListChanged();

		// Register listener
		AGameRoundsTimeline* Timeline = StandardGameState->GetGameRoundsTimeline();
		Timeline->OnTimelinePlayerInfoListChanged.AddDynamic(this, &UGameRoundsTimelineWidget::OnPlayerInfoListChanged);
	}
}

void UGameRoundsTimelineWidget::OnPlayerInfoListChanged()
{
	// Synchronize the PlayerInfo list before call Blueprint implementation
	SynchronizePlayerInfoList();

	OnDisplayPlayerInfoListChanged();
}

void UGameRoundsTimelineWidget::SynchronizePlayerInfoList()
{
	if (AStandardGameState* StandardGameState = GetWorld()->GetGameState<AStandardGameState>())
	{
		DisplayPlayerInfos.Empty();
		
		AGameRoundsTimeline* Timeline = StandardGameState->GetGameRoundsTimeline();
		TArray<FTimelinePlayerInfo> PlayerInfos = Timeline->GetPlayerInfos();
		for (FTimelinePlayerInfo PlayerInfo : PlayerInfos)
		{
			const FTimelineDisplayPlayerInfo NewPlayerInfo{PlayerInfo.PlayerId, PlayerInfo.PlayerName};
			DisplayPlayerInfos.Add(NewPlayerInfo);
		}
	}
}
