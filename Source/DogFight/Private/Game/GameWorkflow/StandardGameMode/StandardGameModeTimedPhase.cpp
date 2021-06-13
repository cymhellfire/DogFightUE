#include "Game/GameWorkflow/StandardGameMode/StandardGameModeTimedPhase.h"

#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"

void UStandardGameModeTimedPhase::SetPhaseDuration(int32 NewDuration)
{
	PhaseDuration = NewDuration;
}

bool UStandardGameModeTimedPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
	{
		TargetGameState = StandardGameState;
		StandardGameState->SetRemainingTime(PhaseDuration);

		ParentStandardGameMode->GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &UStandardGameModeTimedPhase::OnTimerTick,
		ParentStandardGameMode->GetWorldSettings()->GetEffectiveTimeDilation(), true);
	}

	return true;
}

void UStandardGameModeTimedPhase::OnTimerTick()
{
	TargetGameState->SetRemainingTime(TargetGameState->GetRemainingTime() - 1);

	if (TargetGameState->GetRemainingTime() <= 0)
	{
		OnTimerExpired();
	}
}

void UStandardGameModeTimedPhase::OnTimerExpired()
{
	// Clear the timer
	ParentStandardGameMode->GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

	FinishPhase();
}
