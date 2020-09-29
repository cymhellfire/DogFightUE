// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"

#include "DogFightGameStateBase.h"
#include "StandardMiniMapCapture.h"
#include "StandardGameState.generated.h"

class AGameRoundsTimeline;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardGameState : public ADogFightGameStateBase
{
	GENERATED_UCLASS_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePhaseChangeSignature, const FName&, NewGamePhase);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCountdownContentStringChangeSignature);

	UPROPERTY(BlueprintAssignable, Category="DogFight|GameState")
	FGamePhaseChangeSignature OnGamePhaseChanged;

	UPROPERTY(BlueprintAssignable, Category="DogFight|GameState")
	FCountdownContentStringChangeSignature OnCountdownContentStringChanged;

public:
	/* Mini Map Capture Camera */
	TWeakObjectPtr<AStandardMiniMapCapture> MiniMapCamera;

	UPROPERTY(Category=Map, VisibleAnywhere, BlueprintReadOnly)
	/* World bounds for Mini Map and Camera movement. */
	FBox WorldBounds;

	void SetCurrentGamePhase(FName NewGamePhase);

	FORCEINLINE FName GetCurrentGamePhase() const { return CurrentGamePhase; }

	void SetRemainingTime(uint32 Time);

	FORCEINLINE uint32 GetRemainingTime() const { return RemainingTime; }

	void SetCountdownContentString(FString NewString);

	FORCEINLINE FString GetCountdownContentString() const { return CountdownContentString; }

	void RegisterGameRoundTimeline(AGameRoundsTimeline* NewTimeline);

	FORCEINLINE AGameRoundsTimeline* GetGameRoundsTimeline() const { return GameRoundsTimeline; }

protected:
	virtual void BeginPlay() override;

	bool ShouldCountdownDisplay() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, ReplicatedUsing=OnRep_CurrentGamePhase)
	FName CurrentGamePhase;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_RemainingTime)
	int32 RemainingTime;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_CountdownContentString)
	FString CountdownContentString;

	UPROPERTY(VisibleAnywhere, Transient, Replicated)
	AGameRoundsTimeline* GameRoundsTimeline;

	UFUNCTION()
	void OnRep_CurrentGamePhase();

	UFUNCTION()
	void OnRep_RemainingTime();

	UFUNCTION()
	void OnRep_CountdownContentString();
};
