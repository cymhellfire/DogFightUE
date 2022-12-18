#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "GameFlowStateBase.generated.h"

class UGameFlowStateMachine;
class UGameFlowStateCreateArgument;

UCLASS()
class GAMEFLOW_API UGameFlowStateBase : public UObject, public IUnLuaInterface
{
	GENERATED_BODY()
public:
	/**
	 * @brief Execute when enter this state.
	 */
	virtual void OnEnter();

	/**
	 * @brief Execute when leaving this state.
	 */
	virtual void OnExit();

	/**
	 * @brief Execute when a new state interrupted this state.
	 */
	virtual void OnInterrupted();

	/**
	 * @brief Execute when this state resumed.
	 */
	virtual void OnResume();

	virtual void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="GameFlowState")
	void SetCreateArgument(UGameFlowStateCreateArgument* InArgument);

	/**
	 * @brief Mark this state is finished.
	 */
	UFUNCTION(BlueprintCallable, Category="GameFlowState")
	void Finish();

	UFUNCTION(BlueprintCallable, Category="GameFlowState")
	void SetNextState(UGameFlowStateCreateArgument* InArgument);

	UFUNCTION(BlueprintCallable, Category="GameFlowState")
	void InsertState(UGameFlowStateCreateArgument* InArgument);

	UFUNCTION(BlueprintCallable, Category="GameFlowState")
	UGameFlowStateMachine* GetParentStateMachine();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnEnter();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnExit();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnInterrupted();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnResume();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnCreateArgumentSet();

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("GameFlow.GameFlowState.GameFlowState");
	}

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameFlowStateEvent, UGameFlowStateBase*, InState);
	FGameFlowStateEvent OnGameFlowStateFinished;

protected:
	UPROPERTY(Transient)
	UGameFlowStateCreateArgument* CreateArgument;
};
