#pragma once

#include "CoreMinimal.h"
#include "GameFlowStateMachine.generated.h"

class UGameFlowStateBase;
class UGameFlowStateCirculation;

UCLASS()
class GAMEFLOW_API UGameFlowStateMachine : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UGameFlowStateMachine();

	void RegisterGameFlowState(FName InName, UGameFlowStateBase* InState);

	void SwitchState(FName InName);

	void PushState(FName InName);

	void PopState();

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

	void DoStatePush();

	void DoStatePop();

protected:
	UPROPERTY(Transient)
	TMap<FName, UGameFlowStateBase*> GameFlowStateMap;

	UPROPERTY(Transient)
	UGameFlowStateBase* CurrentState;

	FName NextState;

	TWeakObjectPtr<UGameFlowStateCirculation> CurrentCirculation;

	TArray<UGameFlowStateBase*> StateStack;
};
