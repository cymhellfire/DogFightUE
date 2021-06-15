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

	template<typename T>
	T* GetGamePhase(FName PhaseName)
	{
		if (GamePhaseMap.Contains(PhaseName))
		{
			return Cast<T>(GamePhaseMap[PhaseName]);
		}

		return nullptr;
	};

protected:
	UGamePhase* GetNextProcessGamePhase();

	void OnGamePhaseChanged();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGamePhaseChangedSignature, FName, NewGamePhase, uint8, SwitchMethod);
	FGamePhaseChangedSignature OnGamePhaseChangedEvent;

protected:
	bool bPendingSwitch;

	UGamePhase* ProcessingGamePhase;

	AGameModeBase* OwnerGameMode;

	TArray<FName> GamePhaseStack;

	UPROPERTY()
	TMap<FName, UGamePhase*> GamePhaseMap;
};
