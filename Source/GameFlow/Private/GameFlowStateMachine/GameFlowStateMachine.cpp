#include "GameFlowStateMachine/GameFlowStateMachine.h"
#include "GameFlowCommon.h"
#include "GameFlowStateMachine/GameFlowStateCirculation.h"
#include "GameFlowState/GameFlowStateCreateArgument.h"

UGameFlowStateMachine::UGameFlowStateMachine()
{

}

void UGameFlowStateMachine::InitStateMachine()
{
	K2_InitStateMachine();
}

void UGameFlowStateMachine::PushState(UGameFlowStateCreateArgument* InArgument)
{
	if (!IsValid(InArgument))
	{
		return;
	}

	// Create new circulation and push it into stack
	auto NewCirculation = NewObject<UGameFlowStateCirculation>(this);
	NewCirculation->OnCirculationFinished.AddDynamic(this, &UGameFlowStateMachine::OnCirculationFinished);
	NewCirculation->InitCirculation(this, InArgument);
	if (IsValid(CurrentCirculation))
	{
		StateStack.Push(CurrentCirculation);
	}
	CurrentCirculation = NewCirculation;
}

void UGameFlowStateMachine::PopState()
{
	if (StateStack.Num() == 0)
	{
		FinishStateMachine();
		return;
	}

	// Resume popped circulation
	CurrentCirculation = StateStack.Pop();
	CurrentCirculation->ResumeState();
}

void UGameFlowStateMachine::SetNextState(UGameFlowStateCreateArgument* InArgument)
{
	if (IsValid(CurrentCirculation))
	{
		CurrentCirculation->SetNextStateArgument(InArgument);
	}
}

void UGameFlowStateMachine::SetInsertState(UGameFlowStateCreateArgument* InArgument)
{
	if (IsValid(CurrentCirculation))
	{
		CurrentCirculation->InsertNewState(InArgument);
	}
}

void UGameFlowStateMachine::Tick(float DeltaTime)
{
	if (IsValid(CurrentCirculation))
	{
		CurrentCirculation->Tick(DeltaTime);
	}
}

void UGameFlowStateMachine::FinishStateMachine()
{
	UE_LOG(LogGameFlow, Log, TEXT("[GameFlowStateMachine] State machine finished."));
}

void UGameFlowStateMachine::OnCirculationFinished(UGameFlowStateCirculation* InCirculation)
{
	InCirculation->OnCirculationFinished.RemoveDynamic(this, &UGameFlowStateMachine::OnCirculationFinished);

	if (InCirculation != CurrentCirculation)
	{
		UE_LOG(LogGameFlow, Error, TEXT("[GameFlowStateMachine] A circulation that not at stack top is finished."));
		return;
	}

	PopState();
}

FName UGameFlowStateMachine::GetCurrentStateName() const
{
	return IsValid(CurrentCirculation) ? CurrentCirculation->GetCurrentStateName() : NAME_None;
}
