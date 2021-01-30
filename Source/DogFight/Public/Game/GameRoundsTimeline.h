// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "UObject/NoExportTypes.h"
#include "GameRoundsTimeline.generated.h"

class AStandardModePlayerController;
class AStandardModeAIController;

USTRUCT(BlueprintType)
struct FTimelinePlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TimelineIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 FinishedRounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsPlayer;
};

/**
 * Class contains the player action timeline for gameplay.
 */
UCLASS()
class DOGFIGHT_API AGameRoundsTimeline : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimelinePlayerInfoListChangedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimelinePlayerInfoChangedSignature, FTimelinePlayerInfo, PlayerInfo);
	FTimelinePlayerInfoListChangedSignature OnTimelinePlayerInfoListChanged;
	FTimelinePlayerInfoChangedSignature OnTimelinePlayerInfoChanged;

	AGameRoundsTimeline(const FObjectInitializer& ObjectInitializer);

	void RegisterPlayer(AStandardModePlayerController* PlayerController);

	void RegisterAI(AStandardModeAIController* AIController);

	void RenamePlayer(int32 PlayerId, FString NewName);

	void RemovePlayer(int32 PlayerId);

	void RandomizeOrder();

	void SortTimelineByIndex();

	TArray<int32> GetLifetime(int32 RoundCount);

	/** Let the Timeline step forward one player. */
	void StepForward();

	FORCEINLINE TArray<FTimelinePlayerInfo> GetPlayerInfos() const { return TimelinePlayerInfoList; }

	/** Get the Id of player is currently active. */
	FORCEINLINE int32 GetCurrentPlayerId() const { return TimelinePlayerInfoList[0].PlayerId; }

	/** Get the Id of next player. */
	FORCEINLINE int32 GetNextPlayerId() const
	{
		return TimelinePlayerInfoList.Num() > 1 ? TimelinePlayerInfoList[1].PlayerId : GetCurrentPlayerId();
	}

	/** Get if current player is AI controlled. */
	FORCEINLINE bool IsCurrentAIPlayer() const
	{
		return !TimelinePlayerInfoList[0].bIsPlayer;
	}

	void DebugTimeline();

protected:
	UFUNCTION()
	void OnRep_TimelinePlayerInfoList();

	UFUNCTION()
	void OnPlayerDead(int32 PlayerId);

	int32 GetIndexByPlayerId(int32 PlayerId);

protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_TimelinePlayerInfoList)
	TArray<FTimelinePlayerInfo> TimelinePlayerInfoList;

	uint8 bShouldSkipNextStepForward : 1;
};
