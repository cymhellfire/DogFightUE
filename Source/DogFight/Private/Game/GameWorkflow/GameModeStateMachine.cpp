#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "Game/GameWorkflow/GamePhase.h"
#include "DogFight.h"
#include "Game/GameWorkflow/GamePhaseCommon.h"

UGameModeStateMachine::UGameModeStateMachine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PendingProcessGamePhase = NAME_None;
}

void UGameModeStateMachine::BeginDestroy()
{
	GamePhaseStack.Empty();
	GamePhaseMap.Empty();

	Super::BeginDestroy();
}

void UGameModeStateMachine::SetOwnerGameMode(AGameModeBase* InGameMode)
{
	OwnerGameMode = InGameMode;
}

bool UGameModeStateMachine::RegisterGamePhase(UGamePhase* NewGamePhase)
{
	if (!IsValid(NewGamePhase))
	{
		return false;
	}

	if (GamePhaseMap.Contains(NewGamePhase->GamePhaseName))
	{
		UE_LOG(LogDogFight, Error, TEXT("[GameModeStateMachine] Duplicated game state to register [%s]."), *NewGamePhase->GamePhaseName.ToString());
		return false;
	}

	// Record new game phase
	GamePhaseMap.Add(NewGamePhase->GamePhaseName, NewGamePhase);

	// Set owner game mode for new phase
	if (IsValid(OwnerGameMode))
	{
		NewGamePhase->SetOwnerGameMode(OwnerGameMode);
	}
	NewGamePhase->SetOwnerStateMachine(this);

	return true;
}

bool UGameModeStateMachine::RegisterGamePhase(TArray<UGamePhase*> NewGamePhases)
{
	if (NewGamePhases.Num() == 0)
		return false;

	for (int32 Index = 0; Index < NewGamePhases.Num(); ++Index)
	{
		RegisterGamePhase(NewGamePhases[Index]);
	}

	return true;
}

void UGameModeStateMachine::StartWithPhase(FName InitialPhase)
{
	if (!GamePhaseMap.Contains(InitialPhase))
	{
		UE_LOG(LogDogFight, Error, TEXT("[GameModeStateMachine] Cannot found initial phase %s"), *InitialPhase.ToString());
		return;
	}

	GamePhaseStack.Add(InitialPhase);
	OnGamePhaseChanged();
}

void UGameModeStateMachine::ForceJumpToPhase(FName NewPhase)
{
	// Clear pending game phase to ensure next phase is given one
	if (PendingProcessGamePhase != NAME_None)
		PendingProcessGamePhase = NAME_None;

	GamePhaseStack[0] = NewPhase;
	OnGamePhaseChanged();
}

void UGameModeStateMachine::SetNextGamePhase(FName NextPhaseName)
{
	if (PendingProcessGamePhase != NextPhaseName)
	{
		PendingProcessGamePhase = NextPhaseName;
	}
}

void UGameModeStateMachine::SwitchToNextPhase()
{
	if (GamePhaseStack.Num() < 1)
	{
		UE_LOG(LogDogFight, Error, TEXT("[GameModeStateMachine] No valid game process phase in stack."));
		return;
	}

	// Replace the game phase at bottom
	GamePhaseStack[0] = PendingProcessGamePhase;

	// Clear pending state name
	PendingProcessGamePhase = NAME_None;

	OnGamePhaseChanged();
}

void UGameModeStateMachine::PushGamePhase(FName NewPhase)
{
	// Check if new phase meet requirements
	if (GamePhaseMap.Contains(NewPhase) && GamePhaseMap[NewPhase]->GamePhaseType == EGamePhaseType::GPT_Floating)
	{
		GamePhaseStack.Push(NewPhase);
	}

	OnGamePhaseChanged();
}

void UGameModeStateMachine::PopGamePhase()
{
	// Never pop up the last element of game phase stack
	if (GamePhaseStack.Num() < 2)
	{
		return;
	}

	// Pop the last element
	GamePhaseStack.Pop();

	OnGamePhaseChanged();
}

void UGameModeStateMachine::StateMachineTick()
{
	if (PendingProcessGamePhase != NAME_None)
	{
		SwitchToNextPhase();
	}
}

FName UGameModeStateMachine::GetCurrentGamePhaseName() const
{
	return ProcessingGamePhase ? ProcessingGamePhase->GamePhaseName : NAME_None;
}

UGamePhase* UGameModeStateMachine::GetNextProcessGamePhase()
{
	if (GamePhaseStack.Num() == 0)
		return nullptr;

	const FName CurrentGamePhaseName = GamePhaseStack.Last();
	if (!GamePhaseMap.Contains(CurrentGamePhaseName))
	{
		UE_LOG(LogDogFight, Error, TEXT("[GameModeStateMachine] Cannot found phase with name %s"), *CurrentGamePhaseName.ToString());
		return nullptr;
	}

	return GamePhaseMap[CurrentGamePhaseName];
}

void UGameModeStateMachine::OnGamePhaseChanged()
{
	// Get next game phase based on stack
	UGamePhase* NextGamePhase = GetNextProcessGamePhase();

	if (!IsValid(NextGamePhase))
	{
		UE_LOG(LogDogFight, Error, TEXT("[GameModeStateMachine] Invalid next game phase."));
		return;
	}

	// Skip if next game phase equals current one
	if (NextGamePhase == ProcessingGamePhase)
		return;

	uint8 SwitchMethod = SF_None;
	if (IsValid(ProcessingGamePhase))
	{
		// Interrupt origin game phase if not finished yet
		if (!ProcessingGamePhase->bFinished)
		{
			ProcessingGamePhase->InterruptPhase();
			SwitchMethod |= SF_Interrupted;
		}
		else
		{
			ProcessingGamePhase->EndPhase();
		}
	}

	// Switch to next game phase
	ProcessingGamePhase = NextGamePhase;

	// Resume new game phase if it has been interrupted before
	if (ProcessingGamePhase->bInterrupted)
	{
		ProcessingGamePhase->ResumePhase();
		SwitchMethod |= SF_Resumed;
	}
	else
	{
		ProcessingGamePhase->StartPhase();
	}

	OnGamePhaseChangedEvent.Broadcast(ProcessingGamePhase->GamePhaseName, SwitchMethod);
}
