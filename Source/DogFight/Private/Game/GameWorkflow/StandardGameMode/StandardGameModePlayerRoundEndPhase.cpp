#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRoundEndPhase.h"
#include "DogFight.h"
#include "Actors/Managers/BuffQueue.h"
#include "AI/StandardModeAIController.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Player/StandardModePlayerController.h"


bool UStandardGameModePlayerRoundEndPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	TArray<FString> NewRoundEndArguments;

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

		// Disable card selection for ended player
		StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(false);

		// Broadcast event
		ParentStandardGameMode->OnPlayerRoundEnd.Broadcast(CurrentPlayerId);

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Disable all cards using
			StandardPlayerState->MarkAllCardUnUsable();

			NewRoundEndArguments.Add(StandardPlayerState->GetPlayerName());

			// Process buff queue
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(this, &UStandardGameModePlayerRoundEndPhase::OnPlayerBuffQueueEndRoundFinished);
					BuffQueue->StartRoundEndBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueEndRoundFinished();
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

		// End AI player round
		StandardModeAIController->StopAIRound();

		// Broadcast event
		ParentStandardGameMode->OnPlayerRoundEnd.Broadcast(CurrentPlayerId);

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			NewRoundEndArguments.Add(StandardPlayerState->GetPlayerName());

			// Process buff queue
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(this, &UStandardGameModePlayerRoundEndPhase::OnPlayerBuffQueueEndRoundFinished);
					BuffQueue->StartRoundEndBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueEndRoundFinished();
				}
			}
		}
	}

	// Broadcast round end message
	ParentStandardGameMode->BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage {FString(TEXT("TitleMsg_PlayerRoundEnd")), NewRoundEndArguments});

	return true;
}

void UStandardGameModePlayerRoundEndPhase::OnPlayerBuffQueueEndRoundFinished()
{
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
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &UStandardGameModePlayerRoundEndPhase::OnPlayerBuffQueueEndRoundFinished);
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
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &UStandardGameModePlayerRoundEndPhase::OnPlayerBuffQueueEndRoundFinished);
				}
			}
		}
	}

	// Goto next phase
	FinishPhase();
}
