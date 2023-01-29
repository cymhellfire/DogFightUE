#pragma once

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

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameTimelineChanged);
	FOnGameTimelineChanged OnGameTimelineChanged;

protected:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_CurrentTimeline)
	TArray<int32> CurrentTimeline;

	TArray<int32> RandomPriorityList;

	TArray<TSharedPtr<FGameTimelineEntry>> TimelineEntryList;
};
