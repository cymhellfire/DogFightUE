// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "UObject/NoExportTypes.h"
#include "GameRoundsTimeline.generated.h"

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

	void RegisterPlayer(int32 PlayerId, FString PlayerName);

	void RenamePlayer(int32 PlayerId, FString NewName);

	void RemovePlayer(int32 PlayerId);

	void RandomizeOrder();

	void SortTimelineByIndex();

	/** Let the Timeline step forward one player. */
	void StepForward();

	FORCEINLINE TArray<FTimelinePlayerInfo> GetPlayerInfos() const { return TimelinePlayerInfoList; }

	/** Get the Id of player is currently active. */
	FORCEINLINE int32 GetCurrentPlayerId() const { return TimelinePlayerInfoList[0].PlayerId; }

protected:
	UFUNCTION()
	void OnRep_TimelinePlayerInfoList();

protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_TimelinePlayerInfoList)
	TArray<FTimelinePlayerInfo> TimelinePlayerInfoList;
};
