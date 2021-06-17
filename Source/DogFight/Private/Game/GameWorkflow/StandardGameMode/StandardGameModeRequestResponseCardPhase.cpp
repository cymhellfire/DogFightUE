#include "Game/GameWorkflow/StandardGameMode/StandardGameModeRequestResponseCardPhase.h"
#include "DogFight.h"
#include "AI/StandardModeAIController.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Player/StandardModePlayerController.h"
#include "Card/CardBase.h"

UStandardGameModeRequestResponseCardPhase::UStandardGameModeRequestResponseCardPhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GamePhaseType = EGamePhaseType::GPT_Floating;
}

void UStandardGameModeRequestResponseCardPhase::InitializeResponseCardPhase(int32 PlayerId,
	TArray<TSubclassOf<ACardBase>> ResponseAllowCards, AActor* SourceActor, const FText& CardToResponse)
{
	TargetPlayerId = PlayerId;
	ResponseCardClasses = ResponseAllowCards;
	RequestSource = SourceActor;
	CardNameToResponse = CardToResponse;
}

bool UStandardGameModeRequestResponseCardPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	const bool bTargetHumanPlayer = ParentStandardGameMode->IsHumanPlayer(TargetPlayerId);

	if (bTargetHumanPlayer)
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(TargetPlayerId);
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), TargetPlayerId);
			return false;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Apply card using filter
			StandardPlayerState->ClearCardUsableFilter();
			StandardPlayerState->ApplyCardUsableFilterByClass(ResponseCardClasses);
			StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Passive);

			// Check if there is any card can response
			if (StandardPlayerState->GetUsableCardCount() > 0)
			{
				// Face to incoming enemy
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(StandardModePlayerController->GetActualPawn()))
				{
					const FVector FacingDirection = RequestSource->GetActorLocation() - StandardModePlayerCharacter->GetActorLocation();
					StandardModePlayerCharacter->SetAimingDirection(FacingDirection, [this, StandardPlayerState, StandardModePlayerController]()
					{
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Response);
						StandardPlayerState->OnResponseCardSelected.AddDynamic(this, &UStandardGameModeRequestResponseCardPhase::OnResponseCardSelected);
						StandardModePlayerController->ClientStartRequestResponseCard(1, CardNameToResponse);
						StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
					});
				}
			}
			else
			{
				OnResponseCardSelected(nullptr, StandardPlayerState);
			}
		}
	}
	else
	{
		// Handle AI player
		AStandardModeAIController* StandardModeAIController = ParentStandardGameMode->GetAIControllerById(TargetPlayerId);
		if (StandardModeAIController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get AIController with Id %d"), TargetPlayerId);
			return false;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			// Apply card using filter
			StandardPlayerState->ClearCardUsableFilter();
			StandardPlayerState->ApplyCardUsableFilterByClass(ResponseCardClasses);
			StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Passive);

			// Check if there is any card can response
			if (StandardPlayerState->GetUsableCardCount() > 0)
			{
				// Face to incoming enemy
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(StandardModeAIController->GetActualPawn()))
				{
					const FVector FacingDirection = RequestSource->GetActorLocation() - StandardModePlayerCharacter->GetActorLocation();
					StandardModePlayerCharacter->SetAimingDirection(FacingDirection, [this, StandardPlayerState, StandardModeAIController]()
					{
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Response);
						StandardPlayerState->OnResponseCardSelected.AddDynamic(this, &UStandardGameModeRequestResponseCardPhase::OnResponseCardSelected);

						StandardModeAIController->UseResponseCard();
					});
				}
			}
			else
			{
				OnResponseCardSelected(nullptr, StandardPlayerState);
			}
		}
	}

	return true;
}

void UStandardGameModeRequestResponseCardPhase::OnResponseCardSelected(ACardBase* SelectedCard, AStandardPlayerState* ResponsePlayerState)
{
	// Unregister callback
	if (ResponsePlayerState->OnResponseCardSelected.IsAlreadyBound(this, &UStandardGameModeRequestResponseCardPhase::OnResponseCardSelected))
	{
		ResponsePlayerState->OnResponseCardSelected.RemoveDynamic(this, &UStandardGameModeRequestResponseCardPhase::OnResponseCardSelected);
	}

	const int32 PlayerId = ResponsePlayerState->GetPlayerId();
	if (ParentStandardGameMode->IsHumanPlayer(PlayerId))
	{
		if (AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(PlayerId))
		{
			ResponsePlayerState->MarkAllCardUnUsable();
			StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(false);
			StandardModePlayerController->ClientStopRequestResponseCard();
		}
	}

	if (IsValid(SelectedCard))
	{
		// Use card here
		SelectedCard->Use();
	}

	// Broadcast callback
	ParentStandardGameMode->OnPlayerResponseCardSelected.Broadcast();

	// Finish phase
	FinishPhase();
}
