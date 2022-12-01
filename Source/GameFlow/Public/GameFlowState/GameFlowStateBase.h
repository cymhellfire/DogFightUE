#pragma once

#include "GameFlowStateBase.generated.h"

UCLASS()
class GAMEFLOW_API UGameFlowStateBase : public UObject
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

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnEnter();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnExit();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnInterrupted();

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowState")
	void K2_OnResume();
};
