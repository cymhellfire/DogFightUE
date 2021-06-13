#include "Game/GameWorkflow/StandardGameMode/StandardGameModeDiscardCardsPhase.h"
#include "DogFight.h"
#include "AI/StandardModeAIController.h"
#include "Common/BitmaskOperation.h"
#include "Game/GameType.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Player/StandardModePlayerController.h"

bool UStandardGameModeDiscardCardsPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
	ParentStandardGameMode->OnPlayerDiscardCard.Broadcast(CurrentPlayerId);

	if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId);
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), CurrentPlayerId);
			return false;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Check whether to discard cards
			const int32 DiscardCount = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_DropCards) ? 0
				: StandardPlayerState->CardCountToDiscard();
			if (DiscardCount > 0 && StandardPlayerState->IsAlive())
			{
				StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Discard);
				// Enable card selection for discarding
				StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
				StandardPlayerState->ClearCardUsableFilter();
				StandardPlayerState->SetDesireCardCountAfterDiscard(StandardPlayerState->GetMaxCardNum());
				StandardModePlayerController->ClientStartDiscardCards(DiscardCount);

				StandardPlayerState->OnDiscardCardFinished.AddDynamic(this, &UStandardGameModeDiscardCardsPhase::OnPlayerDiscardCardFinished);
			}
			else
			{
				FinishPhase();
			}
		}
	}
	else
	{
		// Handle AI player
		AStandardModeAIController* StandardModeAIController = ParentStandardGameMode->GetAIControllerById(CurrentPlayerId);
		if (StandardModeAIController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get AIController with Id %d"), CurrentPlayerId);
			return false;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			// Check whether to discard cards
			const int32 DiscardCount = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_DropCards) ? 0
				: StandardPlayerState->CardCountToDiscard();
			if (DiscardCount > 0 && StandardPlayerState->IsAlive())
			{
				StandardModeAIController->DiscardRandomCards(DiscardCount);
			}

			FinishPhase();
		}
	}

	return true;
}

void UStandardGameModeDiscardCardsPhase::OnPlayerDiscardCardFinished()
{
	if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
	{
		if (AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(ParentStandardGameMode->GetCurrentPlayerId()))
		{
			StandardModePlayerController->ClientStopDiscardCards();
			if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->OnDiscardCardFinished.RemoveDynamic(this, &UStandardGameModeDiscardCardsPhase::OnPlayerDiscardCardFinished);
			}
		}
	}

	FinishPhase();
}
