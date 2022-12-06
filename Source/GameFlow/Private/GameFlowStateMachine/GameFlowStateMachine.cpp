#include "GameFlowStateMachine/GameFlowStateMachine.h"
#include "GameFlowCommon.h"
#include "GameFlowState/GameFlowStateBase.h"
#include "GameFlowStateMachine/GameFlowStateCirculation.h"

UGameFlowStateMachine::UGameFlowStateMachine()
{

}

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
}

void UGameFlowStateMachine::PushState(FName InName)
{
	if (!GameFlowStateMap.Contains(InName))
	{
		UE_LOG(LogGameFlow, Error, TEXT("[UGameFlowStateMachine] Cannot find game flow state with name: %s"), *InName.ToString());
		return;
	}

	// Record pushed state name for later switching
	NextState = InName;
}

void UGameFlowStateMachine::PopState()
{
	if (StateStack.Num() == 0)
	{
		UE_LOG(LogGameFlow, Error, TEXT("[UGameFlowStateMachine] No state in stack to pop."));
		return;
	}
}

void UGameFlowStateMachine::Tick(float DeltaTime)
{
	if (CurrentCirculation.IsValid())
	{
		CurrentCirculation->Tick(DeltaTime);
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
		NewState->OnEnter(SET_Default);
		CurrentState = NewState;
	}
}

void UGameFlowStateMachine::DoStatePush()
{
	// Interrupt current state
	if (CurrentState)
	{
		CurrentState->OnInterrupted();
	}

	// Enter new state
	if (auto FindPtr = GameFlowStateMap.Find(NextState))
	{
		// Push current state into stack
		StateStack.Push(CurrentState);

		auto NewState = *FindPtr;
		NewState->OnEnter(SET_Interrupt);
		CurrentState = NewState;
	}
}

void UGameFlowStateMachine::DoStatePop()
{
	// Exit current state
	if (CurrentState)
	{
		CurrentState->OnExit();
	}

	// Pop state from stack
	if (auto NewState = StateStack.Pop())
	{
		// Invoke resume of new state
		NewState->OnResume();
		CurrentState = NewState;
	}
	else
	{
		UE_LOG(LogGameFlow, Error, TEXT("[UGameFlowStateMachine] No state to pop in stack."));
	}
}
