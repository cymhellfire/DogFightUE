#pragma once

#include "CoreMinimal.h"
#include "GameFlowStateMachine.generated.h"

class UGameFlowStateBase;
class UGameFlowStateCreateArgument;
class UGameFlowStateCirculation;

UCLASS()
class GAMEFLOW_API UGameFlowStateMachine : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UGameFlowStateMachine();

	void InitStateMachine();

	UFUNCTION(BlueprintCallable, Category="GameFlowStateMachine")
	void PushState(UGameFlowStateCreateArgument* InArgument);

	void PopState();

	void SetNextState(UGameFlowStateCreateArgument* InArgument);

	void SetInsertState(UGameFlowStateCreateArgument* InArgument);

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
	void FinishStateMachine();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowStateMachine")
	void K2_InitStateMachine();

	UFUNCTION()
	void OnCirculationFinished(UGameFlowStateCirculation* InCirculation);

protected:
	UPROPERTY(Transient)
	TArray<UGameFlowStateCirculation*> StateStack;

	UPROPERTY(Transient)
	UGameFlowStateCirculation* CurrentCirculation;
};
