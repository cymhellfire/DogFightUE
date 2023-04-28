#pragma once

#include "GameMode/DataStruct/TimelineRoundTimer.h"
#include "GameTimelineComponent.generated.h"

class FGameTimelineEntry;

UCLASS()
class DOGFIGHTGAME_API UGameTimelineComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UGameTimelineComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * @brief			Initialize the timeline with current players.
	 */
	void InitializeTimeline();

	/**
	 * @brief			Get the current timeline order.
	 * @return			List of player id in timeline order.
	 */
	TArray<int32> GetTimeline() const;

	/**
	 * @brief			Get the first player Id in current order.
	 * @return			The Id of first player in timeline.
	 */
	int32 GetFirstPlayerId() const;

	/**
	 * @brief Move the timeline forward.
	 */
	void MoveForward();

	/**
	 * @brief			Get the waiting queue formed by player id of given duration.
	 * @param InTime	Timer duration in round unit.
	 * @return			Array of player id that represents the whole timer lifetime.
	 */
	TArray<int32> GetTimerWaitingQueue(int32 InTime);

	/**
	 * @brief			Remove a timeline entry by specified player id.
	 * @param InId		Id of entry to be removed.
	 */
	void RemoveEntryByPlayerId(int32 InId);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTimelineEntryRemoved(int32 InId);

	void AddTimer(int32 InTime, FTimelineRoundTimerFinishDelegate& InCallback);

protected:
	/**
	 * @brief					Generate a randomized priority value list with given player number.
	 * @param InPlayerNum		Player number to generate priority value.
	 */
	void InitializeRandomPriorityList(int32 InPlayerNum);

	/**
	 * @brief			Get a priority value from generated list.
	 * @return			Priority value.
	 */
	int32 GetRandomizedPriority();

	void UpdateTimeline();

	UFUNCTION()
	void OnRep_CurrentTimeline();

	void OnRoundTimerExpired(TSharedPtr<FTimelineRoundTimer> InTimer);

public:
	DECLARE_MULTICAST_DELEGATE(FOnGameTimelineChanged);
	FOnGameTimelineChanged OnGameTimelineChanged;

	FOnGameTimelineChanged OnNewRoundStarted;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameTimelineEntryRemoved, int32);
	FOnGameTimelineEntryRemoved OnTimelineEntryRemoved;

protected:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_CurrentTimeline)
	TArray<int32> CurrentTimeline;

	int32 RoundStartPlayerId;

	int32 CurrentRound;

	TArray<int32> RandomPriorityList;

	TArray<TSharedPtr<FGameTimelineEntry>> TimelineEntryList;
	TArray<TSharedPtr<FTimelineRoundTimer>> TimerList;
};
