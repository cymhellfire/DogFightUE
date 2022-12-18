#include "GameService/GameFlowStateService.h"
#include "GameFlowState/GameFlowStateBase.h"
#include "GameFlowState/GameFlowStateCreateArgument.h"

UGameFlowStateBase* UGameFlowStateService::CreateGameFlowState(UGameFlowStateCreateArgument* InArgument)
{
	if (!IsValid(InArgument))
	{
		return nullptr;
	}

	auto NewInstance = NewObject<UGameFlowStateBase>(InArgument->Instigator);
	if (NewInstance)
	{
		NewInstance->SetCreateArgument(InArgument);
	}
	return NewInstance;
}
