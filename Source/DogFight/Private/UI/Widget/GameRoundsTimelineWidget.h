// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameRoundsTimelineWidget.generated.h"

USTRUCT(BlueprintType)
struct FTimelineDisplayPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerId;

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;
};

/**
 * Widget to display the in-game Timeline.
 */
UCLASS()
class UGameRoundsTimelineWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Synchronize the PlayerInfo with AGameRoundTimeline actor and register all event listener. */
	void SetupTimelineDisplay();

	/** Fires when one of the PlayerInfo has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnDisplayPlayerInfoChanged(const FTimelineDisplayPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="DogFight|UI")
	void OnDisplayPlayerInfoListChanged();

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FORCEINLINE TArray<FTimelineDisplayPlayerInfo> GetAllDisplayPlayerInfo() const { return DisplayPlayerInfos; }

protected:
	UFUNCTION()
	void OnPlayerInfoListChanged();

	void SynchronizePlayerInfoList();

protected:

	TArray<FTimelineDisplayPlayerInfo> DisplayPlayerInfos;
};
