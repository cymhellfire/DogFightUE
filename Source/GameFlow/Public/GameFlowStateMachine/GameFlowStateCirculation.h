#pragma once

#include "CoreMinimal.h"
#include "GameFlowStateCirculation.generated.h"

class UGameFlowStateMachine;
class UGameFlowStateBase;

UCLASS()
class GAMEFLOW_API UGameFlowStateCirculation : public UObject
{
	GENERATED_BODY()
public:
	UGameFlowStateCirculation();

	void InitCirculation(UGameFlowStateMachine* InStateMachine);

	void SetNextStateName(FName InName);

	void Tick(float DeltaTime);

protected:
	void DoStateSwitch();

	UFUNCTION()
	void OnGameFlowStateFinished(UGameFlowStateBase* InState);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameFlowStateCirculationEvent, UGameFlowStateCirculation*, Circulation);
	FGameFlowStateCirculationEvent OnCirculationFinished;

protected:
	uint8 bStateFinished : 1;

	FName NextStateName;

	TWeakObjectPtr<UGameFlowStateMachine> ParentStateMachine;
	TWeakObjectPtr<UGameFlowStateBase> CurrentState;
};
