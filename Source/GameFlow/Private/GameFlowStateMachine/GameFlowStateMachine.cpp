#include "GameFlowStateMachine/GameFlowStateMachine.h"
#include "GameFlowCommon.h"
#include "GameFlowState/GameFlowStateBase.h"

void UGameFlowStateMachine::RegisterGameFlowState(FName InName, UGameFlowStateBase* InState)
{
	if (GameFlowStateMap.Contains(InName))
	{
		UE_LOG(LogGameFlow, Error, TEXT("[UGameFlowStateMachine] Duplicated name detected when add new game flow state: %s"), *InName.ToString());
		return;
	}

	GameFlowStateMap.Add(InName, InState);
}

void UGameFlowStateMachine::SwitchState(FName InName)
{
	if (!GameFlowStateMap.Contains(InName))
	{
		UE_LOG(LogGameFlow, Error, TEXT("[UGameFlowStateMachine] Cannot find game flow state with name: %s"), *InName.ToString());
		return;
	}

	// Record next state name for later switching
	NextState = InName;
	bPendingSwitchState = true;
}

void UGameFlowStateMachine::Tick(float DeltaTime)
{
	if (bPendingSwitchState)
	{
		DoStateSwitch();
	}
}

void UGameFlowStateMachine::DoStateSwitch()
{
	// Exit current state
	if (CurrentState)
	{
		CurrentState->OnExit();
	}

	// Enter new state
	if (auto FindPtr = GameFlowStateMap.Find(NextState))
	{
		auto NewState = *FindPtr;
		NewState->OnEnter();
		CurrentState = NewState;
	}

	bPendingSwitchState = false;
}
