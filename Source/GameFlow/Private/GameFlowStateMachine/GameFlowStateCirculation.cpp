#include "GameFlowStateMachine/GameFlowStateCirculation.h"
#include "GameFlowState/GameFlowStateBase.h"
#include "GameFlowCommon.h"
#include "GameFlowState/GameFlowStateCreateArgument.h"
#include "GameFlowStateMachine/GameFlowStateMachine.h"
#include "GameService/GameFlowStateService.h"
#include "GameService/GameService.h"

UGameFlowStateCirculation::UGameFlowStateCirculation()
{
	bStateFinished = false;
}

void UGameFlowStateCirculation::InitCirculation(UGameFlowStateMachine* InStateMachine, UGameFlowStateCreateArgument* InitStateArgument)
{
	ParentStateMachine = InStateMachine;

	// Switch to initial state
	NextStateArgument = InitStateArgument;
	DoStateSwitch();
}

void UGameFlowStateCirculation::SetNextStateArgument(UGameFlowStateCreateArgument* InArgument)
{
	NextStateArgument = InArgument;
}

void UGameFlowStateCirculation::InsertNewState(UGameFlowStateCreateArgument* InArgument)
{
	InsertStateArgument = InArgument;
	bHasStateToInsert = true;
}

void UGameFlowStateCirculation::ResumeState()
{
	if (IsValid(CurrentState))
	{
		CurrentState->OnResume();
	}
}

void UGameFlowStateCirculation::Tick(float DeltaTime)
{
	if (bHasStateToInsert)
	{
		DoStateInsert();
		return;
	}

	if (bStateFinished)
	{
		bStateFinished = false;
		if (IsValid(NextStateArgument))
		{
			DoStateSwitch();
		}
		else
		{
			FinishCirculation();
		}
	}
	else
	{
		// Tick current state
		if (IsValid(CurrentState))
		{
			CurrentState->Tick(DeltaTime);
		}
	}
}

void UGameFlowStateCirculation::DoStateSwitch()
{
	// Exit current state
	if (IsValid(CurrentState))
	{
		CurrentState->OnExit();
	}

	// Create new game flow state and enter
	if (auto GameFlowStateService = UGameService::GetGameService<UGameFlowStateService>())
	{
		CurrentState = GameFlowStateService->CreateGameFlowState(NextStateArgument);

		if (IsValid(CurrentState))
		{
			CurrentState->OnGameFlowStateFinished.AddDynamic(this, &UGameFlowStateCirculation::OnGameFlowStateFinished);
			// Enter new state
			CurrentState->OnEnter();
		}
	}

	NextStateArgument = nullptr;
}

void UGameFlowStateCirculation::DoStateInsert()
{
	if (IsValid(CurrentState))
	{
		CurrentState->OnInterrupted();
	}

	if (ParentStateMachine.IsValid())
	{
		ParentStateMachine->PushState(InsertStateArgument);
	}

	InsertStateArgument = nullptr;
	bHasStateToInsert = false;
}

void UGameFlowStateCirculation::FinishCirculation()
{
	if (IsValid(CurrentState))
	{
		CurrentState->OnExit();
	}

	OnCirculationFinished.Broadcast(this);
}

void UGameFlowStateCirculation::OnGameFlowStateFinished(UGameFlowStateBase* InState)
{
	if (IsValid(InState))
	{
		InState->OnGameFlowStateFinished.RemoveDynamic(this, &UGameFlowStateCirculation::OnGameFlowStateFinished);
	}

	// Mark state finished
	bStateFinished = true;
}
