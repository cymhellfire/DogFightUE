// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GamePhase.generated.h"

UENUM(BlueprintType)
enum class EGamePhaseType : uint8
{
	GPT_Process			UMETA(DisplayName="Process", Tooltip="Fixed game phases that following given order."),
	GPT_Floating		UMETA(DisplayName="Floating", Tooltip="Floating phase that can be entered at anytime."),
};

/**
 * Base class of all game phases.
 */
UCLASS()
class DOGFIGHT_API UGamePhase : public UObject
{
	GENERATED_BODY()
public:
	UGamePhase(const FObjectInitializer& ObjectInitializer);

	friend class UGameModeStateMachine;

	void InitializeGamePhase(FName PhaseName, EGamePhaseType PhaseType, FName FollowingPhase);

	virtual void SetOwnerGameMode(AGameModeBase* InGameMode);

	virtual void SetOwnerStateMachine(UGameModeStateMachine* InStateMachine);

protected:

	virtual bool StartPhase();

	virtual void FinishPhase();

	virtual void EndPhase();

	virtual void InterruptPhase();

	virtual void ResumePhase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GamePhase")
	FName GamePhaseName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GamePhase")
	EGamePhaseType GamePhaseType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GamePhase", meta=(EditCondition="GamePhaseType!=GPT_Floating"))
	FName FollowingGamePhase;

	uint8 bFinished : 1;
	uint8 bInterrupted : 1;

	UGameModeStateMachine* OwnerStateMachine;

	AGameModeBase* OwnerGameMode;
};