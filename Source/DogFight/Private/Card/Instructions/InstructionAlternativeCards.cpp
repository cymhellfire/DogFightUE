// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionAlternativeCards.h"

#include "AI/StandardModeAIController.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Player/StandardModePlayerController.h"

UInstructionAlternativeCards::UInstructionAlternativeCards(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DiscardCount = FUpgradableIntProperty(0, TEXT("Property_DiscardCount"), ECardDisplayInfoLocType::ILT_Card);
	GainCount = FUpgradableIntProperty(2, TEXT("Property_GainCount"), ECardDisplayInfoLocType::ILT_Card);
}

void UInstructionAlternativeCards::Execute()
{
	Super::Execute();

	// Ask user to discard cards
	bool bWaitingForPlayer = false;
	if (DiscardCount.GetValue() > 0)
	{
		if (AController* Controller = GetOwnerCard()->GetOwnerPlayerController())
		{
			if (AStandardModePlayerController* StandardModePlayerController = Cast<AStandardModePlayerController>(Controller))
			{
				TargetPlayerController = StandardModePlayerController;
				TargetPlayerState = Controller->GetPlayerState<AStandardPlayerState>();
				if (IsValid(TargetPlayerState))
				{
					TargetPlayerState->OnDiscardCardFinished.AddDynamic(this, &UInstructionAlternativeCards::OnDiscardCardFinished);
					TargetPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Discard);
					TargetPlayerState->ClearCardUsableFilter();
					TargetPlayerState->SetDesireCardCountAfterDiscard(TargetPlayerState->GetCurrentCardCount() - DiscardCount.GetValue());

					StandardModePlayerController->ClientStartDiscardCards(DiscardCount.GetValue());
					bWaitingForPlayer = true;
				}
				else
				{
					UE_LOG(LogDogFight, Error, TEXT("Invalid player state."));
				}
			}
			else if (AStandardModeAIController* StandardModeAIController = Cast<AStandardModeAIController>(Controller))
			{
				StandardModeAIController->DiscardRandomCards(DiscardCount.GetValue());
			}
		}
	}

	// Give card to user
	if (!bWaitingForPlayer)
	{
		GiveCards(GainCount.GetValue());
	}
}

void UInstructionAlternativeCards::GiveCards(int32 Count)
{
	if (Count > 0)
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			StandardGameMode->GivePlayerCards(TargetPlayerState->GetPlayerId(), Count);
		}

		if (IsValid(TargetPlayerState))
		{
			TargetPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Use);
			TargetPlayerState->ClearCardUsableFilter();
			TargetPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
		}

		if (IsValid(TargetPlayerController))
		{
			TargetPlayerController->ClientSetCardDisplayWidgetSelectable(true);
		}
	}

	Finish();
}

void UInstructionAlternativeCards::OnDiscardCardFinished()
{
	if (IsValid(TargetPlayerState))
	{
		TargetPlayerState->OnDiscardCardFinished.RemoveDynamic(this, &UInstructionAlternativeCards::OnDiscardCardFinished);
	}

	if (IsValid(TargetPlayerController))
	{
		TargetPlayerController->ClientStopDiscardCards();
	}

	GiveCards(GainCount.GetValue());
}


