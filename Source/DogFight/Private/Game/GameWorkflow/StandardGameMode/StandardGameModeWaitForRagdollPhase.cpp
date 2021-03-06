﻿#include "Game/GameWorkflow/StandardGameMode/StandardGameModeWaitForRagdollPhase.h"

#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "Pawns/StandardModePlayerCharacter.h"


UStandardGameModeWaitForRagdollPhase::UStandardGameModeWaitForRagdollPhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GamePhaseType = EGamePhaseType::GPT_Floating;
	RagdollCount = 0;
}

void UStandardGameModeWaitForRagdollPhase::RegisterListenCharacter(AStandardModePlayerCharacter* NewCharacter)
{
	if (ListeningCharacters.Contains(NewCharacter))
		return;

	ListeningCharacters.Add(NewCharacter);

	// Register listener to new characters
	NewCharacter->OnCharacterRagdollStateChanged.AddDynamic(this, &UStandardGameModeWaitForRagdollPhase::OnCharacterRagdollStateChanged);
	NewCharacter->OnCharacterDead.AddDynamic(this, &UStandardGameModeWaitForRagdollPhase::OnCharacterDead);
}

void UStandardGameModeWaitForRagdollPhase::ClearListeningCharacters()
{
	// Clear listeners
	for (int32 Index = 0; Index < ListeningCharacters.Num(); ++Index)
	{
		if (IsValid(ListeningCharacters[Index]))
		{
			ListeningCharacters[Index]->OnCharacterRagdollStateChanged.RemoveDynamic(this, &UStandardGameModeWaitForRagdollPhase::OnCharacterRagdollStateChanged);
			ListeningCharacters[Index]->OnCharacterDead.RemoveDynamic(this, &UStandardGameModeWaitForRagdollPhase::OnCharacterDead);
		}
	}

	ListeningCharacters.Empty();
}

void UStandardGameModeWaitForRagdollPhase::BeginDestroy()
{
	ClearListeningCharacters();

	Super::BeginDestroy();
}

void UStandardGameModeWaitForRagdollPhase::ResumePhase()
{
	Super::ResumePhase();

	// Do once check after resumed
	if (RagdollCount <= 0)
	{
		FinishPhase();
	}
}

void UStandardGameModeWaitForRagdollPhase::OnCharacterDead()
{
	// Dead player won't count
	RagdollCount--;
	if (RagdollCount <= 0)
	{
		FinishPhase();
	}
}

void UStandardGameModeWaitForRagdollPhase::OnCharacterRagdollStateChanged(AStandardModePlayerCharacter* Character, bool bActive)
{
	// Skip counting if character is dead
	if (!Character->IsAlive())
		return;

	if (bActive)
	{
		RagdollCount++;
		// Push this state if any character enter ragdoll state
		if (OwnerStateMachine->GetCurrentGamePhase() != this && RagdollCount > 0)
		{
			OwnerStateMachine->PushGamePhase(GamePhaseName);
		}
	}
	else
	{
		RagdollCount--;
		// Stop waiting if all character recovered
		if (RagdollCount <= 0)
		{
			FinishPhase();
		}
	}
}
