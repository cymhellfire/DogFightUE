#include "GameFlowStateMachine/GameFlowStateCirculation.h"
#include "GameFlowState/GameFlowStateBase.h"
#include "GameFlowCommon.h"
#include "GameService/GameFlowStateService.h"
#include "GameService/GameService.h"

UGameFlowStateCirculation::UGameFlowStateCirculation()
{
	bStateFinished = false;
}

void UGameFlowStateCirculation::InitCirculation(UGameFlowStateMachine* InStateMachine)
{
	ParentStateMachine = InStateMachine;
}

void UGameFlowStateCirculation::SetNextStateName(FName InName)
{
	if (!NextStateName.IsNone())
	{
		UE_LOG(LogGameFlow, Error, TEXT("[GameFlowStateCirculation] Fail to set next state [%s]. Pending state already exsits: %s"),
			*InName.ToString(), *NextStateName.ToString());
		return;
	}

	NextStateName = InName;
}

void UGameFlowStateCirculation::Tick(float DeltaTime)
{
	if (bStateFinished)
	{
		bStateFinished = false;
		if (!NextStateName.IsNone())
		{
			DoStateSwitch();
		}
		else
		{
			OnCirculationFinished.Broadcast(this);
		}
	}
}

void UGameFlowStateCirculation::DoStateSwitch()
{
	// Exit current state
	if (CurrentState.IsValid())
	{
		CurrentState->OnExit();
	}

	// Create new game flow state and enter
	if (auto GameFlowStateService = UGameService::GetGameService<UGameFlowStateService>())
	{
		CurrentState = GameFlowStateService->CreateGameFlowState(NextStateName);

		if (CurrentState.IsValid())
		{
			CurrentState->OnGameFlowStateFinished.AddDynamic(this, &UGameFlowStateCirculation::OnGameFlowStateFinished);
			// Enter new state
			CurrentState->OnEnter();
		}
	}
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
