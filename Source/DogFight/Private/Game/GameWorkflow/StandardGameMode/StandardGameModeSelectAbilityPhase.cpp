#include "Game/GameWorkflow/StandardGameMode/StandardGameModeSelectAbilityPhase.h"
#include "Ability/AbilityBase.h"
#include "Game/GameplayAbilityPool.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Player/StandardModePlayerController.h"


bool UStandardGameModeSelectAbilityPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	// Let all human players to select ability
	for (AStandardModePlayerController* PlayerController : ParentStandardGameMode->GetAllPlayerControllers())
	{
		AStandardPlayerState* StandardPlayerState = PlayerController->GetPlayerState<AStandardPlayerState>();
		if (!IsValid(StandardPlayerState))
			continue;

		TArray<FAbilityDisplayInfo> AbilityDisplayInfos;
		TArray<UAbilityBase*> CandidateAbilities = ParentStandardGameMode->GetAbilityPool()->GetRandomAbility(ParentStandardGameMode->InitialAbilityCandidateCount);
		for (int32 Index = 0; Index < CandidateAbilities.Num(); ++Index)
		{
			UAbilityBase* NewAbility = CandidateAbilities[Index];
			StandardPlayerState->AddCandidateAbility(NewAbility);
			AbilityDisplayInfos.Add(NewAbility->GetAbilityDisplayInfo());
		}
		AbilitySelectingPlayerCount++;
		StandardPlayerState->OnCandidateAbilitySelected. AddDynamic(this, &UStandardGameModeSelectAbilityPhase::OnCandidateAbilitySelected);
		PlayerController->ClientShowAbilitySelectWindow(AbilityDisplayInfos);
	}

	return true;
}

void UStandardGameModeSelectAbilityPhase::OnCandidateAbilitySelected(AStandardPlayerState* PlayerState)
{
	if (IsValid(PlayerState))
	{
		PlayerState->OnCandidateAbilitySelected.RemoveDynamic(this, &UStandardGameModeSelectAbilityPhase::OnCandidateAbilitySelected);
	}

	AbilitySelectingPlayerCount--;
	if (AbilitySelectingPlayerCount <= 0)
	{
		FinishPhase();
	}
}
