// Dog Fight Game Code By CYM.


#include "Game/StandardPlayerState.h"

#include "Ability/AbilityBase.h"
#include "Card/CardBase.h"
#include "Game/StandardGameState.h"
#include "Game/StandardGameMode.h"
#include "Player/StandardModePlayerController.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "AI/StandardModeAIController.h"
#include "Actors/Managers/BuffQueue.h"
#include "Common/BitmaskOperation.h"

AStandardPlayerState::AStandardPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initial value
	bAlive = true;
}

void AStandardPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardPlayerState, CardInfoList);
	DOREPLIFETIME(AStandardPlayerState, UsableCardIndex);
	DOREPLIFETIME(AStandardPlayerState, MaxCardCount);
	DOREPLIFETIME(AStandardPlayerState, MaxUseNum);
	DOREPLIFETIME(AStandardPlayerState, UsedCardNum);
	DOREPLIFETIME(AStandardPlayerState, CardGainPerRounds);
	DOREPLIFETIME(AStandardPlayerState, CardSelectionPurpose);
	DOREPLIFETIME(AStandardPlayerState, bAlive);
	DOREPLIFETIME(AStandardPlayerState, PlayerStatisticArray);
	DOREPLIFETIME(AStandardPlayerState, bIsRagdoll);
	DOREPLIFETIME(AStandardPlayerState, SkipGamePhaseFlags);
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

	CardInstanceList.Add(Card);
	RefreshCardInfoList();

	// Invoke OnRep function on server side
	OnRep_CardInfoList();
}

void AStandardPlayerState::RemoveCard(int32 CardIndex)
{
	if (GetNetMode() == NM_Client)
		return;

	if (CardIndex >= CardInstanceList.Num() || CardIndex < 0)
	{
		return;
	}

	CardInstanceList.RemoveAt(CardIndex);
	RefreshCardInfoList();

	// Invoke OnRep function on server side
	OnRep_CardInfoList();
}

void AStandardPlayerState::ServerHandleSelectedCard_Implementation(int32 Index)
{
	if (Index >= CardInstanceList.Num())
	{
		UE_LOG(LogDogFight, Error, TEXT("Cannot find card instance with index %d"), Index);
		return;
	}

	switch(CardSelectionPurpose)
	{
	case ECardSelectionPurpose::CSP_Use:
		{
			// Begin using specified card
			ACardBase* UsingCard = CardInstanceList[Index];
			UsingCard->Use();
			UsingCardIndex = Index;

			// Register to card's delegate
			UsingCard->OnCardFinished.AddDynamic(this, &AStandardPlayerState::OnCardFinished);
		}
		break;
	case ECardSelectionPurpose::CSP_Discard:
		{
			// Discard specified card from list
			RemoveCard(Index);

			// Callback
			if (CardInstanceList.Num() == MaxCardCount)
			{
				OnDiscardCardFinished.Broadcast();
			}
		}
		break;
	case ECardSelectionPurpose::CSP_Response:
		{
			ACardBase* ResponseCard = CardInstanceList[Index];
			RemoveCard(Index);

			// Callback
			OnResponseCardSelected.Broadcast(ResponseCard, this);
		}
		break;
	default: ;
	}
}

void AStandardPlayerState::ServerHandleEmptySelectCard_Implementation()
{
	switch(CardSelectionPurpose)
	{
	case ECardSelectionPurpose::CSP_Response:
		{
			// Callback
			OnResponseCardSelected.Broadcast(nullptr, this);
		}
		break;
	default: ;
	}
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
	RefreshCardInfoList();

	// Increase used card count
	SetUsedCardCount(UsedCardNum + 1);
	// Check if player round should be ended
	const bool bFinishRound = (UsedCardNum >= MaxUseNum || CardInstanceList.Num() <= 0);
	OnUsingCardFinished.Broadcast(bFinishRound);

	UsingCardIndex = -1;

	// Invoke OnRep on server side
	OnRep_CardInfoList();
}

void AStandardPlayerState::OnAbilityCooldownChanged(int32 AbilitySlot, int32 CurrentCooldown)
{
	OnPlayerAbilityCooldownChanged.Broadcast(AbilitySlot, CurrentCooldown);
}

void AStandardPlayerState::OnAbilityAvailabilityChanged(int32 AbilitySlot, bool NewAvailability)
{
	OnPlayerAbilityAvailabilityChanged.Broadcast(AbilitySlot, NewAvailability);
}

void AStandardPlayerState::InitializePlayerForNewRound()
{
	SetUsedCardCount(0);
}

int32 AStandardPlayerState::GetCardGainNumByRound()
{
	return CardGainPerRounds;
}

void AStandardPlayerState::SetCardGainNumByRound(int32 NewValue)
{
	if (NewValue == CardGainPerRounds)
		return;

	// Clamp the value based on GameMode
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameMode))
		{
			CardGainPerRounds = StandardGameMode->ClampCardGainPerRound(NewValue);

			if (GetNetMode() != NM_Client)
			{
				OnRep_CardGainPerRound();
			}
		}
	}
}

int32 AStandardPlayerState::GetCardUseCountPerRound()
{
	return MaxUseNum;
}

void AStandardPlayerState::SetCardUseCountPerRound(int32 NewValue)
{
	if (NewValue == MaxUseNum)
		return;

	// Clamp the value based on GameMode
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameMode))
		{
			MaxUseNum = StandardGameMode->ClampCardUsingCount(NewValue);

			if (GetNetMode() != NM_Client)
			{
				OnRep_MaxUseNum();
			}
		}
	}
}

int32 AStandardPlayerState::GetUsedCardCount()
{
	return UsedCardNum;
}

void AStandardPlayerState::SetUsedCardCount(int32 NewValue)
{
	if (NewValue == UsedCardNum)
		return;

	UsedCardNum = NewValue;

	if (GetNetMode() != NM_Client)
	{
		OnRep_UsedCardNum();
	}
}

int32 AStandardPlayerState::GetMaxCardNum()
{
	return MaxCardCount;
}

void AStandardPlayerState::SetMaxCardNum(int32 NewValue)
{
	if (NewValue == MaxCardCount)
		return;

	// Clamp the value based on GameMode
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameMode))
		{
			MaxCardCount = StandardGameMode->ClampCardCapacity(NewValue);

			if (GetNetMode() != NM_Client)
			{
				OnRep_MaxCardCount();
			}
		}
	}
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

	UE_LOG(LogDogFight, Log, TEXT("Process GameState with %d players."), StandardGameState->PlayerArray.Num());

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

#if WITH_EDITOR
	OnPlayerRelationInfoChanged.Broadcast();
#endif
}

void AStandardPlayerState::ModifyPlayerHealth(int32 TargetPlayerId, int32 NewHealth)
{
	FPlayerRelationStatistic* TargetStatistic = nullptr;
	if (GetPlayerRelationStatistic(TargetPlayerId, &TargetStatistic))
	{
		TargetStatistic->CurrentHealth = NewHealth;
	}

#if WITH_EDITOR
	OnPlayerRelationInfoChanged.Broadcast();
#endif
}

int32 AStandardPlayerState::GetPlayerCardByClass(TArray<TSubclassOf<ACardBase>> CardClasses, TArray<int32>& CardIndex)
{
	int32 Result = 0;
	CardIndex.Empty();
	for (int32 i = 0; i < CardInstanceList.Num(); ++i)
	{
		UClass* CardClass = CardInstanceList[i]->GetClass();
		if (CardClasses.Contains(CardClass))
		{
			// Record index in result array
			CardIndex.Add(i);
			Result++;
		}
	}

	return Result;
}

bool AStandardPlayerState::GetRandomTransferCards(int32 CardCount, TArray<int32>& CardIndexList)
{
	CardIndexList.Empty();
	if (CardInstanceList.Num() == 0)
	{
		return false;
	}

	// Put all cards into candidate list
	TArray<int32> CandidateList;
	for (int32 Index = 0; Index < CardInstanceList.Num(); ++Index)
	{
		CandidateList.Add(Index);
	}

	if (CardCount >= CardInstanceList.Num())
	{
		// Return all cards as result since no enough cards
		CardIndexList = TArray<int32>(CandidateList);

		return CardCount == CardInstanceList.Num();
	}

	// Get random cards from candidate list
	for (int32 Loop = 0; Loop < CardCount; ++Loop)
	{
		int32 Index = FMath::RandRange(0, CandidateList.Num() - 1);
		CardIndexList.Add(Index);

		// Remove selected card from candidate list
		CandidateList.Remove(Index);
	}

	return true;
}

ACardBase* AStandardPlayerState::GetCardByIndex(int32 CardIndex)
{
	if (CardIndex < CardInstanceList.Num())
	{
		return CardInstanceList[CardIndex];
	}

	return nullptr;
}

void AStandardPlayerState::ApplyCardUsableFilterByClass(TArray<TSubclassOf<ACardBase>> CardClasses)
{
	for (int32 Index = UsableCardIndex.Num() - 1; Index >= 0; --Index)
	{
		UClass* CardClass = CardInstanceList[UsableCardIndex[Index]]->GetClass();
		if (!CardClasses.Contains(CardClass))
		{
			// Filter out card not meet requirement
			UsableCardIndex.RemoveAt(Index);
		}
	}

	if (GetNetMode() != NM_Client)
	{
		OnRep_UsableCardIndex();
	}
}

void AStandardPlayerState::ApplyCardUsableFilterByUseMethod(ECardUseMethod NewUseMethod)
{
	for (int32 Index = UsableCardIndex.Num() - 1; Index >= 0; --Index)
	{
		const ACardBase* CardInstance = CardInstanceList[UsableCardIndex[Index]];
		if (CardInstance->GetCardUseMethod() != NewUseMethod)
		{
			// Filter out card not meet requirement
			UsableCardIndex.RemoveAt(Index);
		}
	}

	if (GetNetMode() != NM_Client)
	{
		OnRep_UsableCardIndex();
	}
}

void AStandardPlayerState::ClearCardUsableFilter()
{
	// Mark all card as usable
	UsableCardIndex.Empty();
	for (int32 Index = 0; Index < CardInstanceList.Num(); ++Index)
	{
		UsableCardIndex.Add(Index);
	}

	if (GetNetMode() != NM_Client)
	{
		OnRep_UsableCardIndex();
	}
}

void AStandardPlayerState::MarkAllCardUnUsable()
{
	// Clear usable cards
	UsableCardIndex.Empty();

	if (GetNetMode() != NM_Client)
	{
		OnRep_UsableCardIndex();
	}
}

void AStandardPlayerState::MarkGamePhasesAsSkip(int32 GamePhaseFlags)
{
	MERGE_FLAGS(SkipGamePhaseFlags, GamePhaseFlags);
}

void AStandardPlayerState::EraseGamePhasesFromSkip(int32 GamePhaseFlags)
{
	REMOVE_FLAGS(SkipGamePhaseFlags, GamePhaseFlags);
}

void AStandardPlayerState::AddAbility(UAbilityBase* NewAbility)
{
	if (Abilities.Contains(NewAbility))
	{
		UE_LOG(LogDogFight, Error, TEXT("[StandardPlayerState] Duplicated ability %s to add."), *NewAbility->GetAbilityName().ToString());
		NewAbility->ConditionalBeginDestroy();
		return;
	}

	NewAbility->RegisterAbility(this);
	NewAbility->SetAbilitySlot(Abilities.Num());
	NewAbility->OnAbilityCooldownChanged.AddDynamic(this, &AStandardPlayerState::OnAbilityCooldownChanged);
	NewAbility->OnAbilityAvailabilityChanged.AddDynamic(this, &AStandardPlayerState::OnAbilityAvailabilityChanged);
	Abilities.Add(NewAbility);

	OnPlayerAbilityAdded.Broadcast(NewAbility->GetAbilityDisplayInfo(), NewAbility->GetAbilitySlot());
}

void AStandardPlayerState::RemoveAbility(FName AbilityToRemove)
{
	int32 TargetIndex = -1;
	for (int32 Index = 0; Index < Abilities.Num(); ++Index)
	{
		if (Abilities[Index]->GetAbilityName() == AbilityToRemove)
		{
			TargetIndex = Index;
			break;
		}
	}

	if (TargetIndex != -1)
	{
		Abilities[TargetIndex]->OnAbilityCooldownChanged.RemoveDynamic(this, &AStandardPlayerState::OnAbilityCooldownChanged);
		Abilities[TargetIndex]->OnAbilityAvailabilityChanged.RemoveDynamic(this, &AStandardPlayerState::OnAbilityAvailabilityChanged);
		Abilities[TargetIndex]->UnregisterAbility();
		Abilities.RemoveAt(TargetIndex);

		// Update all ability slot
		for (int32 Index = 0; Index < Abilities.Num(); ++Index)
		{
			Abilities[Index]->SetAbilitySlot(Index);
		}

		OnPlayerAbilityRemoved.Broadcast(TargetIndex);
	}
}

void AStandardPlayerState::UseAbility(int32 AbilitySlot)
{
	if (AbilitySlot < 0 || AbilitySlot >= Abilities.Num())
	{
		UE_LOG(LogDogFight, Error, TEXT("[PlayerState] Invalid ability slot %d to use."), AbilitySlot);
		return;
	}

	Abilities[AbilitySlot]->Activate();
}

void AStandardPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_Client)
	{
		// Create Buff Queue on server side
		PlayerBuffQueue = NewObject<UBuffQueue>(this, UBuffQueue::StaticClass());

		InitializeCardVariable();
	}
}

void AStandardPlayerState::InitializeCardVariable()
{
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameMode))
		{
			MaxCardCount = StandardGameMode->GetDefaultCardCapacity();
			MaxUseNum = StandardGameMode->GetDefaultCardUsingCount();
			CardGainPerRounds = StandardGameMode->GetDefaultCardGainPerRound();
		}
	}
}

void AStandardPlayerState::RefreshCardInfoList()
{
	CardInfoList.Empty(CardInfoList.Num());
	ValidateCardInstances();

	for (ACardBase* Card : CardInstanceList)
	{
		CardInfoList.Add(Card->GetCardDisplayInfo());
	}
}

void AStandardPlayerState::ValidateCardInstances()
{
	TArray<int32> InvalidIndex;
	for (int32 Index = CardInstanceList.Num() - 1; Index >= 0; --Index)
	{
		if (!IsValid(CardInstanceList[Index]))
		{
			InvalidIndex.Add(Index);
		}
	}

	for (int32 Index : InvalidIndex)
	{
		CardInstanceList.RemoveAt(Index);
	}
}

void AStandardPlayerState::OnRep_CardInfoList()
{
	OnPlayerCardCountChanged.Broadcast(GetCurrentCardCount());

	// Broadcast event
	OnPlayerCardInfoListChanged.Broadcast();

	// Card changing cause using ability changed 
	OnPlayerCardUsingAbilityChanged.Broadcast();
}

void AStandardPlayerState::OnRep_UsableCardIndex()
{
	OnPlayerCardUsableIndexChanged.Broadcast();
}

void AStandardPlayerState::OnRep_SkipGamePhaseFlags()
{
	
}

void AStandardPlayerState::OnRep_MaxCardCount()
{
	OnPlayerCardUsingAbilityChanged.Broadcast();
}

void AStandardPlayerState::OnRep_MaxUseNum()
{
	OnPlayerCardUsingAbilityChanged.Broadcast();
}

void AStandardPlayerState::OnRep_UsedCardNum()
{
	OnPlayerCardUsingAbilityChanged.Broadcast();
}

void AStandardPlayerState::OnRep_CardGainPerRound()
{
	OnPlayerCardUsingAbilityChanged.Broadcast();
}
