#include "GameFlowState/GameFlowStateBase.h"

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
