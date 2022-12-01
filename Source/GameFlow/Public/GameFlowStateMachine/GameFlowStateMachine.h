#pragma once

#include "CoreMinimal.h"
#include "GameFlowStateMachine.generated.h"

class UGameFlowStateBase;

UCLASS()
class GAMEFLOW_API UGameFlowStateMachine : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	void RegisterGameFlowState(FName InName, UGameFlowStateBase* InState);

	void SwitchState(FName InName);

	// FTickableGameObject
	virtual void Tick(float DeltaTime) override;

	virtual UWorld* GetTickableGameObjectWorld() const override
	{
		return GetWorld();
	}

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT( USceneCapturer, STATGROUP_Tickables );
	}

protected:
	void DoStateSwitch();

protected:
	UPROPERTY(Transient)
	TMap<FName, UGameFlowStateBase*> GameFlowStateMap;

	UPROPERTY(Transient)
	UGameFlowStateBase* CurrentState;

	uint8 bPendingSwitchState : 1;

	FName NextState;

	TArray<UGameFlowStateBase*> StateStack;
};
