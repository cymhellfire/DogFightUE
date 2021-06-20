#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRevivePhase.h"

#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "Pawns/StandardModePlayerCharacter.h"

UStandardGameModePlayerRevivePhase::UStandardGameModePlayerRevivePhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GamePhaseType = EGamePhaseType::GPT_Floating;
	RevivingCharacters = 0;
}

void UStandardGameModePlayerRevivePhase::RegisterListenCharacter(AStandardModePlayerCharacter* NewCharacter)
{
	if (ListeningCharacters.Contains(NewCharacter))
		return;

	NewCharacter->OnCharacterReviveStateChanged.AddDynamic(this, &UStandardGameModePlayerRevivePhase::OnCharacterReviveStateChanged);
	ListeningCharacters.Add(NewCharacter);
}

void UStandardGameModePlayerRevivePhase::ClearListeningCharacters()
{
	for (AStandardModePlayerCharacter* StandardModePlayerCharacter : ListeningCharacters)
	{
		if (IsValid(StandardModePlayerCharacter))
		{
			StandardModePlayerCharacter->OnCharacterReviveStateChanged.RemoveDynamic(this, &UStandardGameModePlayerRevivePhase::OnCharacterReviveStateChanged);
		}
	}
	ListeningCharacters.Empty();
}

void UStandardGameModePlayerRevivePhase::BeginDestroy()
{
	ClearListeningCharacters();

	Super::BeginDestroy();
}

void UStandardGameModePlayerRevivePhase::ResumePhase()
{
	Super::ResumePhase();

	// Do once check after resumed
	if (RevivingCharacters <= 0)
	{
		FinishPhase();
	}
}

void UStandardGameModePlayerRevivePhase::OnCharacterReviveStateChanged(AStandardModePlayerCharacter* NewCharacter, bool bActive)
{
	if (bActive)
	{
		// Someone begins to revive
		RevivingCharacters ++;

		// Push this game phase to game state machine
		if (OwnerStateMachine->GetCurrentGamePhase() != this && RevivingCharacters > 0)
		{
			OwnerStateMachine->PushGamePhase(GamePhaseName);
		}
	}
	else
	{
		// Someone finished reviving
		RevivingCharacters --;

		if (RevivingCharacters <= 0)
		{
			FinishPhase();
		}
	}
}
