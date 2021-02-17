// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_SkipGamePhase.h"

#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"

ABuff_SkipGamePhase::ABuff_SkipGamePhase()
	: Super()
{
	LifetimeMode = EBuffLifetimeMode::EBLM_Custom;
}

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

	// Consume lifetime at round begin
	ConsumeLifetime(0);
}

TArray<int32> ABuff_SkipGamePhase::SetupCustomLifetime(int32 TimelineFactor)
{
	TArray<int32> Result;
	// Just add corresponding count of '0' to queue
	for (int32 i = 0; i < TimelineFactor; ++i)
	{
		Result.Add(0);
	}

	return Result;
}
