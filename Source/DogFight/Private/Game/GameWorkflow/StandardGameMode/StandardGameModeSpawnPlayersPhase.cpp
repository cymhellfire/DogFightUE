#include "Game/GameWorkflow/StandardGameMode/StandardGameModeSpawnPlayersPhase.h"
#include "DogFight.h"
#include "Game/DogFightGameInstance.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Player/StandardModePlayerController.h"
#include "AI/StandardModeAIController.h"
#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePhaseDefine.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRevivePhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeWaitForRagdollPhase.h"

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

void UStandardGameModeSpawnPlayersPhase::FinishPhase()
{
	// Try to setup 'WaitForRagdoll' game phase here
	if (UStandardGameModeWaitForRagdollPhase* WaitForRagdollPhase = OwnerStateMachine->GetGamePhase<UStandardGameModeWaitForRagdollPhase>(StandardGameModePhase::WaitForRagdoll))
	{
		for (AStandardModePlayerController* PlayerController : ParentStandardGameMode->GetAllPlayerControllers())
		{
			WaitForRagdollPhase->RegisterListenCharacter(PlayerController->GetCharacterPawn());
		}

		for (AStandardModeAIController* AIController : ParentStandardGameMode->GetAllAIControllers())
		{
			WaitForRagdollPhase->RegisterListenCharacter(AIController->GetCharacterPawn());
		}
	}

	// Try to setup 'PlayerRevive' game phase here
	if (UStandardGameModePlayerRevivePhase* PlayerRevivePhase = OwnerStateMachine->GetGamePhase<UStandardGameModePlayerRevivePhase>(StandardGameModePhase::PlayerRevive))
	{
		for (AStandardModePlayerController* PlayerController : ParentStandardGameMode->GetAllPlayerControllers())
		{
			PlayerRevivePhase->RegisterListenCharacter(PlayerController->GetCharacterPawn());
		}

		for (AStandardModeAIController* AIController : ParentStandardGameMode->GetAllAIControllers())
		{
			PlayerRevivePhase->RegisterListenCharacter(AIController->GetCharacterPawn());
		}
	}

	Super::FinishPhase();
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
