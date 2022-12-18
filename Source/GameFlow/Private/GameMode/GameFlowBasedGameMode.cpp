#include "GameMode/GameFlowBasedGameMode.h"
#include "GameFlowStateMachine/GameFlowStateMachine.h"
#include "GameService/GameFlowStateService.h"

void AGameFlowBasedGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Create new state machine
	if (auto GameFlowStateService = UGameService::GetGameService<UGameFlowStateService>())
	{
		if (!StateMachineName.IsEmpty())
		{
			GameFlowStateMachine = GameFlowStateService->CreateStateMachine(StateMachineName, this);
			if (GameFlowStateMachine)
			{
				GameFlowStateMachine->InitStateMachine();
			}
		}
	}
}

void AGameFlowBasedGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Tick game flow state machine
	if (IsValid(GameFlowStateMachine))
	{
		GameFlowStateMachine->Tick(DeltaSeconds);
	}
}
