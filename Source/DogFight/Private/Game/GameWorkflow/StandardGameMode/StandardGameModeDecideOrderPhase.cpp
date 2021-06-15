#include "Game/GameWorkflow/StandardGameMode/StandardGameModeDecideOrderPhase.h"
#include "Actors/Weapons/WeaponBase.h"
#include "AI/StandardModeAIController.h"
#include "Game/GameRoundsTimeline.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Player/StandardModePlayerController.h"

UStandardGameModeDecideOrderPhase::UStandardGameModeDecideOrderPhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WeaponEquipWaitingCharacterCount = 0;
}

bool UStandardGameModeDecideOrderPhase::StartPhase()
{
	if (!Super::StartPhase())
	{
		return false;
	}

	if (AStandardGameState* StandardGameState = ParentStandardGameMode->GetGameState<AStandardGameState>())
	{
		StandardGameState->GetGameRoundsTimeline()->RandomizeOrder();
		StandardGameState->GetGameRoundsTimeline()->SortTimelineByIndex();

		// Debug Timeline
		StandardGameState->GetGameRoundsTimeline()->DebugTimeline();

		// Check if current is AI player
		ParentStandardGameMode->SetCurrentAIPlayer(StandardGameState->GetGameRoundsTimeline()->IsCurrentAIPlayer());
		ParentStandardGameMode->SetCurrentPlayerId(StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId());

		// Let all clients setup their Timeline widget
		for (AStandardModePlayerController* PlayerController : ParentStandardGameMode->GetAllPlayerControllers())
		{
			PlayerController->ClientSetupTimelineDisplay();

			// Register players statistic
			if (AStandardPlayerState* StandardPlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->RegisterPlayersForRelation();
			}

			// Equip default weapon
			if (IsValid(ParentStandardGameMode->CharacterDefaultWeapon))
			{
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(PlayerController->GetActualPawn()))
				{
					UWeaponBase* NewWeapon = NewObject<UWeaponBase>(StandardModePlayerCharacter, ParentStandardGameMode->CharacterDefaultWeapon);
					StandardModePlayerCharacter->OnWeaponEquippedEvent.AddDynamic(this, &UStandardGameModeDecideOrderPhase::OnWeaponEquipped);
					StandardModePlayerCharacter->EquipWeapon(NewWeapon);

					// Also update cache location
					StandardModePlayerCharacter->CacheCurrentLocation();

					WeaponEquipWaitingCharacterCount++;
				}
			}
		}

		// Register players statistic for AI
		for (AStandardModeAIController* AIController : ParentStandardGameMode->GetAllAIControllers())
		{
			if (AStandardPlayerState* StandardPlayerState = AIController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->RegisterPlayersForRelation();
			}

			// Equip default weapon
			if (IsValid(ParentStandardGameMode->CharacterDefaultWeapon))
			{
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(AIController->GetActualPawn()))
				{
					UWeaponBase* NewWeapon = NewObject<UWeaponBase>(StandardModePlayerCharacter, ParentStandardGameMode->CharacterDefaultWeapon);
					StandardModePlayerCharacter->OnWeaponEquippedEvent.AddDynamic(this, &UStandardGameModeDecideOrderPhase::OnWeaponEquipped);
					StandardModePlayerCharacter->EquipWeapon(NewWeapon);

					// Also update cache location
					StandardModePlayerCharacter->CacheCurrentLocation();

					WeaponEquipWaitingCharacterCount++;
				}
			}
		}
	}

	// If no weapon to equip, just move to next phase
	if (WeaponEquipWaitingCharacterCount == 0)
	{
		FinishPhase();
	}

	return true;
}

void UStandardGameModeDecideOrderPhase::OnWeaponEquipped(AActor* CarrierActor)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(CarrierActor))
	{
		StandardModePlayerCharacter->OnWeaponEquippedEvent.RemoveDynamic(this, &UStandardGameModeDecideOrderPhase::OnWeaponEquipped);
	}

	WeaponEquipWaitingCharacterCount--;
	if (WeaponEquipWaitingCharacterCount == 0)
	{
		FinishPhase();
	}
}
