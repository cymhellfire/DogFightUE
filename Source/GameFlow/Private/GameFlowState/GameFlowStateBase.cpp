#include "GameFlowState/GameFlowStateBase.h"

#include "GameFlowState/GameFlowStateCreateArgument.h"
#include "GameFlowStateMachine/GameFlowStateMachine.h"

void UGameFlowStateBase::OnEnter()
{
	K2_OnEnter();
}

void UGameFlowStateBase::OnExit()
{
	K2_OnExit();
}

void UGameFlowStateBase::OnInterrupted()
{
	K2_OnInterrupted();
}

void UGameFlowStateBase::OnResume()
{
	K2_OnResume();
}

void UGameFlowStateBase::Tick(float DeltaTime)
{
	K2_OnTick(DeltaTime);
}

void UGameFlowStateBase::SetCreateArgument(UGameFlowStateCreateArgument* InArgument)
{
	CreateArgument = InArgument;

	// Trigger blueprint callback
	K2_OnCreateArgumentSet();
}

void UGameFlowStateBase::Finish()
{
	OnGameFlowStateFinished.Broadcast(this);
}

void UGameFlowStateBase::SetNextState(UGameFlowStateCreateArgument* InArgument)
{
	if (auto ParentStateMachine = GetParentStateMachine())
	{
		ParentStateMachine->SetNextState(InArgument);
	}
}

void UGameFlowStateBase::InsertState(UGameFlowStateCreateArgument* InArgument)
{
	if (auto ParentStateMachine = GetParentStateMachine())
	{
		ParentStateMachine->SetInsertState(InArgument);
	}
}

UGameFlowStateMachine* UGameFlowStateBase::GetParentStateMachine()
{
	return GetTypedOuter<UGameFlowStateMachine>();
}

FName UGameFlowStateBase::GetStateName() const
{
	return IsValid(CreateArgument) ? CreateArgument->StateName : NAME_None;
}
