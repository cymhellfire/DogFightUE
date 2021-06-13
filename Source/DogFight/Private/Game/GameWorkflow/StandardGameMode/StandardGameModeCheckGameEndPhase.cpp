#include "Game/GameWorkflow/StandardGameMode/StandardGameModeCheckGameEndPhase.h"

#include "DogFight.h"
#include "Game/GameRoundsTimeline.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Game/GameWorkflow/GameModeStateMachine.h"

void UStandardGameModeCheckGameEndPhase::SetEndGamePhase(FName PhaseName)
{
	if (PhaseName == NAME_None)
	{
		UE_LOG(LogDogFight, Error, TEXT("[GamePhase] Invalid End Game Phase name None"));
		return;
	}
	EndGamePhaseName = PhaseName;
}

bool UStandardGameModeCheckGameEndPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	FinishPhase();

	return true;
}

void UStandardGameModeCheckGameEndPhase::FinishPhase()
{
	bFinished = true;

	if (GamePhaseType == EGamePhaseType::GPT_Process)
	{
		// Check current alive players
		if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
		{
			if (StandardGameState->GetAlivePlayerCount() <= 1)
			{
				OwnerStateMachine->SetNextGamePhase(EndGamePhaseName);
			}
			else
			{
				StandardGameState->GetGameRoundsTimeline()->StepForward();
				// Update game mode
				ParentStandardGameMode->SetCurrentAIPlayer(StandardGameState->GetGameRoundsTimeline()->IsCurrentAIPlayer());
				ParentStandardGameMode->SetCurrentPlayerId(StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId());

				OwnerStateMachine->SetNextGamePhase(FollowingGamePhase);
			}
		}
	}
}
