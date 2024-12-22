#include "GameFlowState/GameFlowStateBase.h"

#include "GameFlowCommon.h"
#include "Common/GlobalLog.h"
#include "GameFlowState/GameFlowStateCreateArgument.h"
#include "GameFlowStateMachine/GameFlowStateCirculation.h"
#include "GameFlowStateMachine/GameFlowStateMachine.h"

void UGameFlowStateBase::OnEnter()
{
	DFLog(LogGameFlow, TEXT("OnEnter %s"), *GetStateName().ToString());
	K2_OnEnter();
}

void UGameFlowStateBase::OnExit()
{
	DFLog(LogGameFlow, TEXT("OnExit %s"), *GetStateName().ToString());
	K2_OnExit();
}

void UGameFlowStateBase::OnInterrupted()
{
	DFLog(LogGameFlow, TEXT("OnInterrupted %s"), *GetStateName().ToString());
	K2_OnInterrupted();
}

void UGameFlowStateBase::OnResume()
{
	DFLog(LogGameFlow, TEXT("OnResume %s"), *GetStateName().ToString());
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
	DFLog(LogGameFlow, TEXT("Finish %s"), *GetStateName().ToString());
	OnGameFlowStateFinished.Broadcast(this);
}

void UGameFlowStateBase::SetNextState(UGameFlowStateCreateArgument* InArgument)
{
	if (ParentCirculation.IsValid())
	{
		DFLog(LogGameFlow, TEXT("SetNextState %s"), *InArgument->StateName.ToString());
		ParentCirculation->SetNextStateArgument(InArgument);
	}
	// if (auto ParentStateMachine = GetParentStateMachine())
	// {
	// 	ParentStateMachine->SetNextState(InArgument);
	// }
}

void UGameFlowStateBase::InsertState(UGameFlowStateCreateArgument* InArgument)
{
	if (ParentCirculation.IsValid())
	{
		DFLog(LogGameFlow, TEXT("InsertState %s"), *InArgument->StateName.ToString());
		ParentCirculation->InsertNewState(InArgument);
	}
	// if (auto ParentStateMachine = GetParentStateMachine())
	// {
	// 	DFLog(LogGameFlow, TEXT("InsertState %s"), *InArgument->StateName.ToString());
	// 	ParentStateMachine->SetInsertState(InArgument);
	// }
}

UGameFlowStateMachine* UGameFlowStateBase::GetParentStateMachine()
{
	return GetTypedOuter<UGameFlowStateMachine>();
}

FName UGameFlowStateBase::GetStateName() const
{
	return IsValid(CreateArgument) ? CreateArgument->StateName : NAME_None;
}
