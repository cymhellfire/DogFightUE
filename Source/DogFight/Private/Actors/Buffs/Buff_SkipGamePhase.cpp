// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_SkipGamePhase.h"

#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"

void ABuff_SkipGamePhase::ApplyBuff()
{
	Super::ApplyBuff();

	if (APawn* Pawn = Cast<APawn>(TargetActor))
	{
		if (AStandardPlayerState* StandardPlayerState = Pawn->GetPlayerState<AStandardPlayerState>())
		{
			StandardPlayerState->MarkGamePhasesAsSkip(SkipGamePhase);
		}
	}
}

void ABuff_SkipGamePhase::RemoveBuff()
{
	if (APawn* Pawn = Cast<APawn>(TargetActor))
	{
		if (AStandardPlayerState* StandardPlayerState = Pawn->GetPlayerState<AStandardPlayerState>())
		{
			StandardPlayerState->EraseGamePhasesFromSkip(SkipGamePhase);
		}
	}

	Super::RemoveBuff();
}

void ABuff_SkipGamePhase::OnTargetPlayerRoundBegin()
{
	Super::OnTargetPlayerRoundBegin();

	// Display the floating text
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		StandardModePlayerCharacter->MulticastAddFloatingText(TriggeredFloatingText.GetLocalizeText());
	}
}
