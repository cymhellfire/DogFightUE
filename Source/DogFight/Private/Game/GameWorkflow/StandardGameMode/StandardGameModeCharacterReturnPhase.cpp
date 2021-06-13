#include "Game/GameWorkflow/StandardGameMode/StandardGameModeCharacterReturnPhase.h"
#include "DogFight.h"
#include "AI/StandardModeAIController.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Player/StandardModePlayerController.h"

bool UStandardGameModeCharacterReturnPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
	if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId);
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), CurrentPlayerId);
			return false;
		}

		// Disable all cards for selecting
		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			StandardPlayerState->MarkAllCardUnUsable();
		}
	}

	ReturnedCharacterCount = 0;

	// Let all players return
	for (AStandardModePlayerController* PlayerController : ParentStandardGameMode->GetAllPlayerControllers())
	{
		if (AStandardPlayerState* StandardPlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
		{
			if (StandardPlayerState->IsAlive())
			{
				if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(PlayerController->GetActualPawn()))
				{
					PlayerCharacter->GetCarrierReachActionDistanceEvent().AddDynamic(this, &UStandardGameModeCharacterReturnPhase::OnCharacterReturnFinished);
					PlayerCharacter->ReturnToCachedLocation();
				}
			}
		}
	}

	// Let all AI return
	for (AStandardModeAIController* AIController : ParentStandardGameMode->GetAllAIControllers())
	{
		if (AStandardPlayerState* StandardPlayerState = AIController->GetPlayerState<AStandardPlayerState>())
		{
			if (StandardPlayerState->IsAlive())
			{
				if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(
					AIController->GetActualPawn()))
				{
					PlayerCharacter->GetCarrierReachActionDistanceEvent().AddDynamic(this, &UStandardGameModeCharacterReturnPhase::OnCharacterReturnFinished);
					PlayerCharacter->ReturnToCachedLocation();
				}
			}
		}
	}

	return true;
}

void UStandardGameModeCharacterReturnPhase::OnCharacterReturnFinished(AActor* Actor)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Actor))
	{
		StandardModePlayerCharacter->GetCarrierReachActionDistanceEvent().RemoveDynamic(this, &UStandardGameModeCharacterReturnPhase::OnCharacterReturnFinished);
	}

	ReturnedCharacterCount++;

	if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
	{
		if (StandardGameState->GetAlivePlayerCount() == ReturnedCharacterCount)
		{
			FinishPhase();
		}
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to get StandardGameState."));
	}
}
