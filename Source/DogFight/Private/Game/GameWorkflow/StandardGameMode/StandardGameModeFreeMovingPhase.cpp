#include "Game/GameWorkflow/StandardGameMode/StandardGameModeFreeMovingPhase.h"

#include "Game/StandardGameMode.h"

bool UStandardGameModeFreeMovingPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	// Enable click move for all players
	ParentStandardGameMode->EnablePlayerClickMovement();

	return true;
}

void UStandardGameModeFreeMovingPhase::EndPhase()
{
	// Disable click move for all players
	ParentStandardGameMode->DisablePlayerClickMovement();

	Super::EndPhase();
}
