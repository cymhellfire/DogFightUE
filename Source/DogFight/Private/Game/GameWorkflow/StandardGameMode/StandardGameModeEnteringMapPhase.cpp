#include "Game/GameWorkflow/StandardGameMode/StandardGameModeEnteringMapPhase.h"

#include "GameInstance/DogFightGameInstance.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"

bool UStandardGameModeEnteringMapPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	ReadyCount = 0;
	if (UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(ParentStandardGameMode->GetGameInstance()))
	{
		DesiredPlayerCount = GameInstance->GamePlayerCount;
	}

	ParentStandardGameMode->OnPlayerReadyForGame.AddDynamic(this, &UStandardGameModeEnteringMapPhase::OnPlayerReadyForGame);

	return true;
}

void UStandardGameModeEnteringMapPhase::OnPlayerReadyForGame()
{
	ReadyCount++;
	if (ReadyCount >= DesiredPlayerCount)
	{
		ParentStandardGameMode->OnPlayerReadyForGame.RemoveDynamic(this, &UStandardGameModeEnteringMapPhase::OnPlayerReadyForGame);
		FinishPhase();
	}
	else
	{
		// Update the countdown content string
		if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
		{
			StandardGameState->SetCountdownContentString(FString::Printf(TEXT("%d/%d"), ReadyCount, DesiredPlayerCount));
		}
	}
}
