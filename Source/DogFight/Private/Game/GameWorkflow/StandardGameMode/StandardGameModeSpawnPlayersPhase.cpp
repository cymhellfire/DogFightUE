#include "Game/GameWorkflow/StandardGameMode/StandardGameModeSpawnPlayersPhase.h"
#include "DogFight.h"
#include "Game/DogFightGameInstance.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Player/StandardModePlayerController.h"
#include "AI/StandardModeAIController.h"

bool UStandardGameModeSpawnPlayersPhase::StartPhase()
{
	if (!Super::StartPhase())
		return false;

	// Send GameStart message
	const FGameMessage NewMessage{TEXT("Server"), EGameMessageType::GMT_System, TEXT("GameMsg_GameStart")};
	ParentStandardGameMode->BroadcastGameMessageToAllPlayers(NewMessage);

	CurrentSpawnPlayerIndex = 0;
	if (UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(ParentStandardGameMode->GetGameInstance()))
	{
		AICount = GameInstance->GameAICount;
	}
	ParentStandardGameMode->GetWorldTimerManager().SetTimer(SpawnPlayerTimerHandle, this, &UStandardGameModeSpawnPlayersPhase::SpawnPlayerTick,
		ParentStandardGameMode->SpawnPlayerInterval, true);

	return true;
}

void UStandardGameModeSpawnPlayersPhase::StartSpawnAI()
{
	CurrentSpawnPlayerIndex = 0;
	ParentStandardGameMode->GetWorldTimerManager().SetTimer(SpawnPlayerTimerHandle, this, &UStandardGameModeSpawnPlayersPhase::SpawnAITick,
		ParentStandardGameMode->SpawnPlayerInterval, true);
}

void UStandardGameModeSpawnPlayersPhase::SpawnPlayerTick()
{
	TArray<AStandardModePlayerController*> AllPlayerControllers = ParentStandardGameMode->GetAllPlayerControllers();
	// Skip and clear the timer if the index is out of range
	if (CurrentSpawnPlayerIndex >= AllPlayerControllers.Num())
	{
		ParentStandardGameMode->GetWorldTimerManager().ClearTimer(SpawnPlayerTimerHandle);

		if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
		{
			StandardGameState->SetAlivePlayerCount(AllPlayerControllers.Num());
		}

		// Go to next phase
		if (AICount > 0)
		{
			StartSpawnAI();
		}
		else
		{
			FinishPhase();
		}
		return;
	}

	// Spawn character for current player
	AllPlayerControllers[CurrentSpawnPlayerIndex]->GameStart();
	CurrentSpawnPlayerIndex++;
}

void UStandardGameModeSpawnPlayersPhase::SpawnAITick()
{
	if (CurrentSpawnPlayerIndex >= AICount)
	{
		ParentStandardGameMode->GetWorldTimerManager().ClearTimer(SpawnPlayerTimerHandle);
		
		if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
		{
			StandardGameState->SetAlivePlayerCount(StandardGameState->GetAlivePlayerCount() + AICount);
		}

		FinishPhase();
		return;
	}

	// Spawn AI Controller
	if (IsValid(ParentStandardGameMode->AIControllerClass))
	{
		GetWorld()->SpawnActor(ParentStandardGameMode->AIControllerClass);
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("No valid AIController class specified."));
	}
	CurrentSpawnPlayerIndex++;
}
