#include "Game/GameWorkflow/StandardGameMode/StandardGameModeGameSummaryPhase.h"

#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Game/StandardPlayerState.h"
#include "Player/StandardModePlayerController.h"

bool UStandardGameModeGameSummaryPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	// Let all player hide the card display widget
	for (AStandardModePlayerController* StandardModePlayerController : ParentStandardGameMode->GetAllPlayerControllers())
	{
		StandardModePlayerController->ClientHideCardDisplayWidget();
	}

	// Get the winner
	AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>();
	AStandardPlayerState* WinnerState = nullptr;
	if (StandardGameState->GetAlivePlayerCount() > 0)
	{
		for (APlayerState* PlayerState: StandardGameState->PlayerArray)
		{
			if (AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerState))
			{
				if (StandardPlayerState->IsAlive())
				{
					WinnerState = StandardPlayerState;
					break;
				}
			}
		}
	}
	// Broadcast winner message
	TArray<FText> Arguments;
	if (WinnerState != nullptr)
	{
		Arguments.Add(FText::FromString(FString::Printf(TEXT("<PlayerName>%s</>"),*WinnerState->GetPlayerName())));
		const FGameMessage WinnerMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_Winner"), Arguments};
		ParentStandardGameMode->BroadcastGameMessageToAllPlayers(WinnerMessage);
	}
	else
	{
		// Consider no survivor as a draw
		const FGameMessage DrawMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_Draw"), Arguments};
		ParentStandardGameMode->BroadcastGameMessageToAllPlayers(DrawMessage);
	}

	return true;
}
