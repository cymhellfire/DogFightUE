// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStateMachine.generated.h"

class UGamePhase;

/**
 * Finite state machine for game mode.
 */
UCLASS()
class DOGFIGHT_API UGameModeStateMachine : public UObject
{
	GENERATED_BODY()
public:
	UGameModeStateMachine(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;

	void SetOwnerGameMode(AGameModeBase* InGameMode);

	bool RegisterGamePhase(UGamePhase* NewGamePhase);

	bool RegisterGamePhase(TArray<UGamePhase*> NewGamePhases);

	void StartWithPhase(FName InitialPhase);

	void ForceJumpToPhase(FName NewPhase);

	void SetNextGamePhase(FName NextPhaseName);

	void SwitchToNextPhase();

	void PushGamePhase(FName NewPhase);

	void PopGamePhase();

	void StateMachineTick();

	UGamePhase* GetCurrentGamePhase()
	{
		return ProcessingGamePhase;
	}

	FName GetCurrentGamePhaseName() const;

	bool HasPendingGamePhase() const
	{
		return PendingProcessGamePhase != NAME_None;
	}

protected:
	UGamePhase* GetNextProcessGamePhase();

	void OnGamePhaseChanged();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePhaseChangedSignature, FName, NewGamePhase);
	FGamePhaseChangedSignature OnGamePhaseChangedEvent;

protected:
	FName PendingProcessGamePhase;

	UGamePhase* ProcessingGamePhase;

	AGameModeBase* OwnerGameMode;

	TArray<FName> GamePhaseStack;

	UPROPERTY()
	TMap<FName, UGamePhase*> GamePhaseMap;
};
