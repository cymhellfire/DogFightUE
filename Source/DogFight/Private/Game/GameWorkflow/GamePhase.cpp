#include "Game/GameWorkflow/GamePhase.h"
#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "DogFight.h"

UGamePhase::UGamePhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	GamePhaseName(NAME_None),
	GamePhaseType(EGamePhaseType::GPT_Process),
	bFinished(false),
	bInterrupted(false)
{
}

void UGamePhase::InitializeGamePhase(FName PhaseName, EGamePhaseType PhaseType, FName FollowingPhase)
{
	GamePhaseName = PhaseName;
	GamePhaseType = PhaseType;
	FollowingGamePhase = FollowingPhase;
}

void UGamePhase::SetOwnerGameMode(AGameModeBase* InGameMode)
{
	OwnerGameMode = InGameMode;
}

void UGamePhase::SetOwnerStateMachine(UGameModeStateMachine* InStateMachine)
{
	OwnerStateMachine = InStateMachine;
}

bool UGamePhase::StartPhase()
{
	UE_LOG(LogDogFight, Log, TEXT("[GamePhase] Start Phase: %s"), *GamePhaseName.ToString());

	bFinished = false;
	bInterrupted = false;

	return true;
}

void UGamePhase::FinishPhase()
{
	bFinished = true;

	if (IsValid(OwnerStateMachine))
	{
		if (GamePhaseType == EGamePhaseType::GPT_Process)
		{
			// Set next phase to state machine automatically if this is a process type phase
			OwnerStateMachine->SetNextGamePhase(FollowingGamePhase);
		}
		else if (GamePhaseType == EGamePhaseType::GPT_Floating)
		{
			// Pop this phase from state machine automatically
			OwnerStateMachine->PopGamePhase();
		}
	}
}

void UGamePhase::EndPhase()
{
	UE_LOG(LogDogFight, Log, TEXT("[GamePhase] End Phase: %s"), *GamePhaseName.ToString());
}

void UGamePhase::InterruptPhase()
{
	bInterrupted = true;

	UE_LOG(LogDogFight, Log, TEXT("[GamePhase] Interrupt Phase: %s"), *GamePhaseName.ToString());
}

void UGamePhase::ResumePhase()
{
	bInterrupted = false;

	UE_LOG(LogDogFight, Log, TEXT("[GamePhase] Resume Phase: %s"), *GamePhaseName.ToString());
}
