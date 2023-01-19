#pragma once

#include "CoreMinimal.h"
#include "GameFlowStateCirculation.generated.h"

class UGameFlowStateMachine;
class UGameFlowStateBase;
class UGameFlowStateCreateArgument;

UCLASS()
class GAMEFLOW_API UGameFlowStateCirculation : public UObject
{
	GENERATED_BODY()
public:
	UGameFlowStateCirculation();

	void InitCirculation(UGameFlowStateMachine* InStateMachine, UGameFlowStateCreateArgument* InitStateArgument);

	void SetNextStateArgument(UGameFlowStateCreateArgument* InArgument);

	/**
	 * @brief Pause current state and insert a new state immediately.
	 * @param InArgument New state name.
	 */
	void InsertNewState(UGameFlowStateCreateArgument* InArgument);

	/**
	 * @brief Resume interrupted state before.
	 */
	void ResumeState();

	void Tick(float DeltaTime);

	FName GetCurrentStateName() const;

protected:
	void DoStateSwitch();

	void DoStateInsert();

	void FinishCirculation();

	UFUNCTION()
	void OnGameFlowStateFinished(UGameFlowStateBase* InState);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameFlowStateCirculationEvent, UGameFlowStateCirculation*, Circulation);
	FGameFlowStateCirculationEvent OnCirculationFinished;

protected:
	uint8 bStateFinished : 1;
	uint8 bHasStateToInsert : 1;

	UPROPERTY(Transient)
	UGameFlowStateCreateArgument* NextStateArgument;
	UPROPERTY(Transient)
	UGameFlowStateCreateArgument* InsertStateArgument;

	TWeakObjectPtr<UGameFlowStateMachine> ParentStateMachine;

	UPROPERTY(Transient)
	UGameFlowStateBase* CurrentState;
};
