#pragma once

class UGameTimelineComponent;
struct FTimelineRoundTimer;

DECLARE_DELEGATE(FTimelineRoundTimerFinishDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FTimelineRoundTimerExpiredDelegate, TSharedPtr<FTimelineRoundTimer>);

struct FTimelineRoundTimer : public TSharedFromThis<FTimelineRoundTimer>
{
public:
	FTimelineRoundTimer();
	~FTimelineRoundTimer();

	void InitTimer(UGameTimelineComponent* InTimelineComponent, int32 InTime, FTimelineRoundTimerFinishDelegate& InCallback);

private:
	void OnTimelineChanged();
	void OnTimelineEntryRemoved(int32 RemovedId);

	void CheckTimer();

	void Finish();
public:
	FTimelineRoundTimerExpiredDelegate OnTimerExpired;

private:
	TArray<int32> WaitingRoundQueue;

	FDelegateHandle OnTimelineChangeHandle;
	FDelegateHandle OnTimelineEntryRemovedHandle;

	FTimelineRoundTimerFinishDelegate FinishDelegate;

	TWeakObjectPtr<UGameTimelineComponent> ListeningTimeline;
};
