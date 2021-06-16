#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRoundPhase.h"

#include "AI/StandardModeAIController.h"
#include "Common/BitmaskOperation.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Game/StandardPlayerState.h"
#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "Player/StandardModePlayerController.h"

void UStandardGameModePlayerRoundPhase::SetPhaseAfterRequestFinish(FName NewPhase)
{
	PhaseAfterRequestFinish = NewPhase;
}

bool UStandardGameModePlayerRoundPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	bRequestFinish = false;
	bPendingCardFinishEvent = false;
	bPendingRoundFinishEvent = false;

	// Listen request finish round message
	ParentStandardGameMode->OnRequestEndCurrentPlayerRound.AddDynamic(this, &UStandardGameModePlayerRoundPhase::OnRequestFinishRound);

	const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
	if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
	{
		if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
		{
			// Show card use Widget
			//AStandardModePlayerController* StandardModePlayerController = ParentGameMode->GetPlayerControllerById(StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId());
			AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId);
			if (StandardModePlayerController != nullptr)
			{
				bool bSkipUsingCardPhase = false;
				if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
				{
					bSkipUsingCardPhase = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_UseCards);

					if (!bSkipUsingCardPhase)
					{
						// Register card finished delegate
						StandardPlayerState->OnUsingCardFinished.AddDynamic(this, &UStandardGameModePlayerRoundPhase::OnPlayerUsingCardFinished);
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Use);
						StandardPlayerState->ClearCardUsableFilter();
						StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
					}
				}

				// Directly end current round if player is marked as SkipUsingCard
				if (!bSkipUsingCardPhase)
				{
					// Enable card selection for new player
					StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
					StandardModePlayerController->ClientShowCardDisplayWidgetWithSelectMode(ECardSelectionMode::CSM_SingleNoConfirm);
				}
				else
				{
					FinishPhase();
				}
			}
		}
		else
		{
			AStandardModeAIController* StandardModeAIController = ParentStandardGameMode->GetAIControllerById(CurrentPlayerId);
			if (StandardModeAIController != nullptr)
			{
				bool bSkipUsingCard = false;
				if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
				{
					bSkipUsingCard = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_UseCards);

					if (!bSkipUsingCard)
					{
						// Register card finished delegate
						StandardPlayerState->OnUsingCardFinished.AddDynamic(this, &UStandardGameModePlayerRoundPhase::OnPlayerUsingCardFinished);
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Use);
						StandardPlayerState->ClearCardUsableFilter();
						StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
					}
				}

				// Directly end current round if player is marked as SkipUsingCard
				if (!bSkipUsingCard)
				{
					// Notify AIController round started
					StandardModeAIController->StartAIRound();
				}
				else
				{
					FinishPhase();
				}
			}
		}
	}

	return true;
}

void UStandardGameModePlayerRoundPhase::EndPhase()
{
	// Remove the card finished delegate
	const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
	if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
	{
		AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId);
		if (IsValid(StandardModePlayerController))
		{
			if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->OnUsingCardFinished.RemoveDynamic(this, &UStandardGameModePlayerRoundPhase::OnPlayerUsingCardFinished);
			}
		}
	}
	else
	{
		AStandardModeAIController* StandardModeAIController = ParentStandardGameMode->GetAIControllerById(CurrentPlayerId);
		if (IsValid(StandardModeAIController))
		{
			if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->OnUsingCardFinished.RemoveDynamic(this, &UStandardGameModePlayerRoundPhase::OnPlayerUsingCardFinished);
			}
		}
	}

	ParentStandardGameMode->OnRequestEndCurrentPlayerRound.RemoveDynamic(this, &UStandardGameModePlayerRoundPhase::OnRequestFinishRound);

	Super::EndPhase();
}

void UStandardGameModePlayerRoundPhase::FinishPhase()
{
	bFinished = true;

	if (GamePhaseType == EGamePhaseType::GPT_Process)
	{
		if (IsValid(OwnerStateMachine))
		{
			OwnerStateMachine->SetNextGamePhase(bRequestFinish ? PhaseAfterRequestFinish : FollowingGamePhase);
		}
	}
}

void UStandardGameModePlayerRoundPhase::ResumePhase()
{
	Super::ResumePhase();

	// Execute card finish event after resume
	if (bPendingCardFinishEvent)
	{
		HandleUsingCardFinishedEvent(bPendingRoundFinishEvent);
	}
}

void UStandardGameModePlayerRoundPhase::HandleUsingCardFinishedEvent(bool bShouldEndRound)
{
	if (bShouldEndRound)
	{
		FinishPhase();
	}
	else
	{
		const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
		if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
		{
			// Re-enable the card display widget using functionality
			if (AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId))
			{
				StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
				if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
				{
					StandardPlayerState->ClearCardUsableFilter();
					StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
				}
			}
		}
		else
		{
			if (AStandardModeAIController* StandardModeAIController = ParentStandardGameMode->GetAIControllerById(CurrentPlayerId))
			{
				if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
				{
					StandardPlayerState->ClearCardUsableFilter();
					StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
				}
				StandardModeAIController->PrepareForUsingCard();
			}
		}
	}
}

void UStandardGameModePlayerRoundPhase::OnRequestFinishRound()
{
	bRequestFinish = true;

	FinishPhase();
}

void UStandardGameModePlayerRoundPhase::OnPlayerUsingCardFinished(bool bShouldEndRound)
{
	if (bInterrupted)
	{
		// Record that a card finish event has been delayed
		bPendingCardFinishEvent = true;
		bPendingRoundFinishEvent = bShouldEndRound;
	}
	else
	{
		HandleUsingCardFinishedEvent(bShouldEndRound);
	}
}
