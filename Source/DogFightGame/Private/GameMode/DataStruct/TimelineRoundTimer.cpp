#include "GameMode/DataStruct/TimelineRoundTimer.h"

#include "GameMode/GameStateComponent/GameTimelineComponent.h"

FTimelineRoundTimer::FTimelineRoundTimer()
{
}

FTimelineRoundTimer::~FTimelineRoundTimer()
{
	// Unregister callback functions
	if (ListeningTimeline.IsValid())
	{
		if (OnTimelineChangeHandle.IsValid())
		{
			ListeningTimeline->OnGameTimelineChanged.Remove(OnTimelineChangeHandle);
		}
		if (OnTimelineEntryRemovedHandle.IsValid())
		{
			ListeningTimeline->OnTimelineEntryRemoved.Remove(OnTimelineEntryRemovedHandle);
		}
	}
}

void FTimelineRoundTimer::InitTimer(UGameTimelineComponent* InTimelineComponent, int32 InTime, FTimelineRoundTimerFinishDelegate& InCallback)
{
	if (!IsValid(InTimelineComponent))
	{
		return;
	}

	// Register callback functions
	ListeningTimeline = InTimelineComponent;
	OnTimelineChangeHandle = InTimelineComponent->OnGameTimelineChanged.AddRaw(this, &FTimelineRoundTimer::OnTimelineChanged);
	OnTimelineEntryRemovedHandle = InTimelineComponent->OnTimelineEntryRemoved.AddRaw(this, &FTimelineRoundTimer::OnTimelineEntryRemoved);

	// Get the waiting queue
	WaitingRoundQueue = ListeningTimeline->GetTimerWaitingQueue(InTime);

	// Record the callback
	FinishDelegate = MoveTemp(InCallback);
}


void FTimelineRoundTimer::OnTimelineChanged()
{
	if (WaitingRoundQueue.Num() > 0)
	{
		// Remove one element from waiting queue once timeline changed
		WaitingRoundQueue.RemoveAt(0);
	}

	CheckTimer();
}

void FTimelineRoundTimer::OnTimelineEntryRemoved(int32 RemovedId)
{
	// Remove all elements has the same value
	WaitingRoundQueue.RemoveAll([RemovedId](int32 InValue)
	{
		return InValue == RemovedId;
	});

	CheckTimer();
}

void FTimelineRoundTimer::CheckTimer()
{
	if (WaitingRoundQueue.Num() == 0)
	{
		Finish();
	}
}

void FTimelineRoundTimer::Finish()
{
	FinishDelegate.ExecuteIfBound();

	// Broadcast
	OnTimerExpired.Broadcast(SharedThis(this));
}
