#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRoundBeginPhase.h"
#include "DogFight.h"
#include "Actors/Managers/BuffQueue.h"
#include "AI/StandardModeAIController.h"
#include "Common/BitmaskOperation.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "Player/StandardModePlayerController.h"

void UStandardGameModePlayerRoundBeginPhase::SetPhaseAfterRequestFinish(FName NewPhase)
{
	PhaseAfterRequestFinish = NewPhase;
}

bool UStandardGameModePlayerRoundBeginPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	bRequestFinish = false;

	// Listen to finish round request
	ParentStandardGameMode->OnRequestEndCurrentPlayerRound.AddDynamic(this, &UStandardGameModePlayerRoundBeginPhase::OnRequestFinishRound);

	const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
	ParentStandardGameMode->OnPrePlayerRoundBegin.Broadcast(CurrentPlayerId);
	// Start Buff Queue process
	if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId);
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), ParentStandardGameMode->GetCurrentPlayerId());
			return false;
		}

		// Camera focus event
		const FVector CurrentLoc = StandardModePlayerController->GetActualPawn()->GetActorLocation();
		ParentStandardGameMode->BroadcastCameraFocusEvent(
			FCameraFocusEvent
			{
				-1,
				CurrentLoc.X,
				CurrentLoc.Y,
				ECameraFocusEventType::Type::Default
			});

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<
			AStandardPlayerState>())
		{
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(
						this, &UStandardGameModePlayerRoundBeginPhase::OnPlayerBuffQueueBeginRoundFinished);
					BuffQueue->StartRoundBeginBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueBeginRoundFinished();
				}
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

		// Camera focus event
		const FVector CurrentLoc = StandardModeAIController->GetActualPawn()->GetActorLocation();
		ParentStandardGameMode->BroadcastCameraFocusEvent(
			FCameraFocusEvent
			{
				-1,
				CurrentLoc.X,
				CurrentLoc.Y,
				ECameraFocusEventType::Type::Default
			});

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<
			AStandardPlayerState>())
		{
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(
						this, &UStandardGameModePlayerRoundBeginPhase::OnPlayerBuffQueueBeginRoundFinished);
					BuffQueue->StartRoundBeginBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueBeginRoundFinished();
				}
			}
		}
	}

	return true;
}

void UStandardGameModePlayerRoundBeginPhase::EndPhase()
{
	// Just remove the delegate
	const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
	if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId);
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), CurrentPlayerId);
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->OnBuffQueueProcessFinished.IsBound())
				{
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &UStandardGameModePlayerRoundBeginPhase::OnPlayerBuffQueueBeginRoundFinished);
				}
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
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->OnBuffQueueProcessFinished.IsBound())
				{
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &UStandardGameModePlayerRoundBeginPhase::OnPlayerBuffQueueBeginRoundFinished);
				}
			}
		}
	}

	ParentStandardGameMode->OnRequestEndCurrentPlayerRound.RemoveDynamic(this, &UStandardGameModePlayerRoundBeginPhase::OnRequestFinishRound);

	Super::EndPhase();
}

void UStandardGameModePlayerRoundBeginPhase::FinishPhase()
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

void UStandardGameModePlayerRoundBeginPhase::OnPlayerBuffQueueBeginRoundFinished()
{
	TArray<FString> NewRoundMessageArgument;

	const int32 CurrentPlayerId = ParentStandardGameMode->GetCurrentPlayerId();
	if (!ParentStandardGameMode->GetIsCurrentAIPlayer())
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = ParentStandardGameMode->GetPlayerControllerById(CurrentPlayerId);
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), CurrentPlayerId);
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Reset player state for new round
			StandardPlayerState->InitializePlayerForNewRound();

			// Skip give card if player is marked with SkipGiveCard
			if (!TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_GiveCards))
			{
				// Give current player random cards
				ParentStandardGameMode->GivePlayerCards(CurrentPlayerId, StandardPlayerState->GetCardGainNumByRound());
			}

			NewRoundMessageArgument.Add(StandardPlayerState->GetPlayerName());
		}
	}
	else
	{
		// Handle AI player
		AStandardModeAIController* StandardModeAIController = ParentStandardGameMode->GetAIControllerById(CurrentPlayerId);
		if (StandardModeAIController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get AIController with Id %d"), CurrentPlayerId);
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			// Reset player state for new round
			StandardPlayerState->InitializePlayerForNewRound();

			// Skip give card if player is marked with SkipGiveCard
			if (!TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_GiveCards))
			{
				// Give current player random cards
				ParentStandardGameMode->GivePlayerCards(CurrentPlayerId, StandardPlayerState->GetCardGainNumByRound());
			}

			NewRoundMessageArgument.Add(StandardPlayerState->GetPlayerName());
		}
	}

	// Broadcast title message
	ParentStandardGameMode->BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage {FString(TEXT("TitleMsg_PlayerRoundBegin")), NewRoundMessageArgument});

	ParentStandardGameMode->OnPlayerRoundBegin.Broadcast(CurrentPlayerId);

	FinishPhase();
}

void UStandardGameModePlayerRoundBeginPhase::OnRequestFinishRound()
{
	bRequestFinish = true;

	FinishPhase();
}
