// Dog Fight Game Code By CYM.


#include "StandardPlayerState.h"
#include "CardBase.h"
#include "StandardGameState.h"
#include "StandardGameMode.h"
#include "StandardModePlayerController.h"
#include "StandardModePlayerCharacter.h"
#include "StandardModeAIController.h"

AStandardPlayerState::AStandardPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initial value
	CardGainPerRounds = 2;
	MaxUseNum = 2;
	bAlive = true;
}

void AStandardPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardPlayerState, CardInfoList);
	DOREPLIFETIME(AStandardPlayerState, MaxUseNum);
	DOREPLIFETIME(AStandardPlayerState, UsedCardNum);
	DOREPLIFETIME(AStandardPlayerState, CardGainPerRounds);
	DOREPLIFETIME(AStandardPlayerState, bAlive);
	DOREPLIFETIME(AStandardPlayerState, PlayerStatisticArray);
	DOREPLIFETIME(AStandardPlayerState, bIsRagdoll);
}

void AStandardPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	// Trigger the delegate
	OnPlayerNameChanged.Broadcast(GetPlayerName());
}

void AStandardPlayerState::AddCard(ACardBase* Card)
{
	if (GetNetMode() == NM_Client)
		return;

	CardInfoList.Add(Card->GetCardDisplayInfo());
	CardInstanceList.Add(Card);

	// Invoke OnRep function on server side
	OnRep_CardInfoList();
}

void AStandardPlayerState::CmdUseCardByIndex_Implementation(int32 Index)
{
	if (Index >= CardInstanceList.Num())
	{
		UE_LOG(LogDogFight, Error, TEXT("Cannot find card instance with index %d"), Index);
		return;
	}

	// Begin using specified card
	ACardBase* UsingCard = CardInstanceList[Index];
	UsingCard->Use();
	UsingCardIndex = Index;

	// Register to card's delegate
	UsingCard->OnCardFinished.AddDynamic(this, &AStandardPlayerState::OnCardFinished);
}

void AStandardPlayerState::OnCardFinished()
{
	ACardBase* UsingCard = CardInstanceList[UsingCardIndex];
	UsingCard->OnCardFinished.RemoveDynamic(this, &AStandardPlayerState::OnCardFinished);
	// Register tick for ragdoll check
	GetWorldTimerManager().SetTimer(RagdollWaitingTimerHandle, this, &AStandardPlayerState::RagdollWaitingTick, 0.1f, true);
}

void AStandardPlayerState::RagdollWaitingTick()
{
	// Check all alive players' character are out of Ragdoll state
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (StandardGameMode->IsAllPlayerNotRagdoll())
		{
			GetWorldTimerManager().ClearTimer(RagdollWaitingTimerHandle);

			PostCardFinished();
		}
	}
}

void AStandardPlayerState::PostCardFinished()
{
	CardInstanceList.RemoveAt(UsingCardIndex);
	CardInfoList.RemoveAt(UsingCardIndex);

	// Increase used card count
	UsedCardNum++;
	// Check if player round should be ended
	const bool bFinishRound = (UsedCardNum >= MaxUseNum || CardInstanceList.Num() <= 0);
	OnUsingCardFinished.Broadcast(bFinishRound);

	UsingCardIndex = -1;

	// Invoke OnRep on server side
	OnRep_CardInfoList();
}

void AStandardPlayerState::InitializePlayerForNewRound()
{
	UsedCardNum = 0;
}

int32 AStandardPlayerState::GetCardGainNumByRound()
{
	return CardGainPerRounds;
}

void AStandardPlayerState::SetAlive(bool bIsAlive)
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	bAlive = bIsAlive;

	if (!bIsAlive)
	{
		// Dead player should be removed from Ragdoll list
		if (bIsRagdoll)
		{
			if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
			{
				StandardGameMode->RemovePlayerInRagdoll(GetPlayerId());
			}
		}

		// Stop the waiting tick timer if player dead
		if (RagdollWaitingTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(RagdollWaitingTimerHandle);

			PostCardFinished();
		}
	}
}

void AStandardPlayerState::SetRagdollActive(bool bActive)
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	bIsRagdoll = bActive;

	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (bActive)
		{
			// No need to record dead player to list
			if (bAlive)
			{
				StandardGameMode->AddPlayerInRagdoll(GetPlayerId());
			}
		}
		else
		{
			StandardGameMode->RemovePlayerInRagdoll(GetPlayerId());
		}
	}
}

void AStandardPlayerState::RegisterPlayersForRelation()
{
	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogDogFight, Error, TEXT("Cannot register players on client side."));
		return;
	}

	AStandardGameState* StandardGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (StandardGameState == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to get GameState"));
		return;
	}

	for (APlayerState* TargetPlayerState : StandardGameState->PlayerArray)
	{
		// Skip myself
		if (TargetPlayerState->GetPlayerId() == GetPlayerId())
		{
			continue;
		}

		AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode());
		if (StandardGameMode == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get GameMode."));
			return;
		}

		const int32 TargetPlayerId = TargetPlayerState->GetPlayerId();
		int32 CurrentHealth = 0;

		if (TargetPlayerState->IsABot())
		{
			AStandardModeAIController* AIController = StandardGameMode->GetAIControllerById(TargetPlayerId);
			CurrentHealth = AIController->GetCharacterPawn()->GetCurrentHealth();
		}
		else
		{
			AStandardModePlayerController* PlayerController = StandardGameMode->GetPlayerControllerById(TargetPlayerId);
			CurrentHealth = PlayerController->GetCharacterPawn()->GetCurrentHealth();
		}

		const FPlayerRelationStatistic PlayerRelationStatistic{TargetPlayerId, TargetPlayerState->IsABot(), 0, 0, CurrentHealth};
		PlayerStatisticArray.Add(PlayerRelationStatistic);
	}
}

bool AStandardPlayerState::GetPlayerRelationStatistic(int32 TargetPlayerId, FPlayerRelationStatistic** PlayerRelationStatistic)
{
	bool bFound = false;
	int32 Index = -1;
	for (int32 i = 0; i < PlayerStatisticArray.Num(); ++i)
	{
		if (PlayerStatisticArray[i].PlayerId == TargetPlayerId)
		{
			bFound = true;
			Index = i;
			break;
		}
	}

	if (bFound)
	{
		*PlayerRelationStatistic = &PlayerStatisticArray[Index];
	}

	return bFound;
}

void AStandardPlayerState::RecordReceivedDamage(int32 SourcePlayerId, float Damage)
{
	FPlayerRelationStatistic* TargetStatistic = nullptr;
	if (GetPlayerRelationStatistic(SourcePlayerId, &TargetStatistic))
	{
		TargetStatistic->ReceiveDamage += (int32)Damage;

		// Modify relation point
		TargetStatistic->RelationPoint -= (int32)Damage;
	}
}

void AStandardPlayerState::ModifyPlayerHealth(int32 TargetPlayerId, int32 NewHealth)
{
	FPlayerRelationStatistic* TargetStatistic = nullptr;
	if (GetPlayerRelationStatistic(TargetPlayerId, &TargetStatistic))
	{
		TargetStatistic->CurrentHealth = NewHealth;
	}
}

void AStandardPlayerState::OnRep_CardInfoList()
{
	// Broadcast event
	OnPlayerCardInfoListChanged.Broadcast();
}
