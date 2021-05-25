// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "Ability/AbilityDisplayInfo.h"
#include "GameFramework/PlayerState.h"
#include "Card/GameCardTypes.h"
#include "Game/GameType.h"
#include "StandardPlayerState.generated.h"

class ACardBase;
class UBuffQueue;
class UAbilityBase;

/**
 * Struct that handles the data describe relationship between players.
 */
USTRUCT(BlueprintType)
struct FPlayerRelationStatistic
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAIPlayer;

	/** Represents Enemy if this value less than 0. Represents Ally when larger than 0. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 RelationPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ReceiveDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentHealth;
};

UENUM(BlueprintType)
enum class ECardSelectionPurpose : uint8
{
	CSP_Use			UMETA(DisplayName="Use"),
	CSP_Discard		UMETA(DisplayName="Discard"),
	CSP_Response	UMETA(DisplayName="Response"),
};

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerNameChangedSignature, const FString&, NewName);
	FPlayerNameChangedSignature OnPlayerNameChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerStateDelegateNoArgument);
	FPlayerStateDelegateNoArgument OnPlayerCardInfoListChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerAbilityAddedSignature, FAbilityDisplayInfo, AbilityInfo, int32, AbilitySlot);
	FPlayerAbilityAddedSignature OnPlayerAbilityAdded;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerAbilityRemovedSignature, int32, AbilitySlot);
	FPlayerAbilityRemovedSignature OnPlayerAbilityRemoved;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerAbilityCooldownChangedSignature, int32, AbilitySlot, int32, CurrentCooldown);
	FPlayerAbilityCooldownChangedSignature OnPlayerAbilityCooldownChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerAbilityAvailabilityChangedSignature, int32, AbilitySlot, bool, NewAvailability);
	FPlayerAbilityAvailabilityChangedSignature OnPlayerAbilityAvailabilityChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUsingCardFinsishedSignature, bool, bPlayerRoundFinished);
	FUsingCardFinsishedSignature OnUsingCardFinished;

	FPlayerStateDelegateNoArgument OnDiscardCardFinished;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseCardSelectedSignature, ACardBase*, SelectedCard, AStandardPlayerState*, PlayerState);
	FResponseCardSelectedSignature OnResponseCardSelected;

	/** Triggered when player using card ability changed, for example, MaxUseNum changed. */
	FPlayerStateDelegateNoArgument OnPlayerCardUsingAbilityChanged;

	FPlayerStateDelegateNoArgument OnPlayerCardUsableIndexChanged;

	AStandardPlayerState(const FObjectInitializer& ObjectInitializer);

	virtual void OnRep_PlayerName() override;

	void AddCard(ACardBase* Card);

	void RemoveCard(int32 CardIndex);

	void SetCardSelectionPurpose(ECardSelectionPurpose NewPurpose)
	{
		CardSelectionPurpose = NewPurpose;
	}

	UFUNCTION(Server, Reliable)
	void ServerHandleSelectedCard(int32 Index);

	UFUNCTION(Server, Reliable)
	void ServerHandleEmptySelectCard();

	FORCEINLINE bool CanUseCard() const { return UsedCardNum < MaxUseNum; }

	/** Initialize the player for his new round. */
	void InitializePlayerForNewRound();

	/** Get the number of cards can be gained in this round. */
	int32 GetCardGainNumByRound();

	void SetCardGainNumByRound(int32 NewValue);

	int32 GetCardUseCountPerRound();

	void SetCardUseCountPerRound(int32 NewValue);

	int32 GetUsedCardCount();

	void SetUsedCardCount(int32 NewValue);

	int32 GetMaxCardNum();

	void SetMaxCardNum(int32 NewValue);

	FORCEINLINE TArray<FCardInstanceDisplayInfo> GetCardDisplayInfoList() const { return CardInfoList; }

	/** Get the card instance list. */
	FORCEINLINE TArray<ACardBase*> GetCardInstanceList() const { return CardInstanceList; }

	void SetAlive(bool bIsAlive);

	FORCEINLINE bool IsAlive() const { return bAlive; }

	void SetRagdollActive(bool bActive);

	/** Get the count of cards this player has. */
	UFUNCTION(BlueprintCallable, Category="StandardPlayerState")
	FORCEINLINE int32 GetCurrentCardCount() const
	{
		return GetNetMode() == NM_Client ? CardInfoList.Num() : CardInstanceList.Num();
	}

	/** Whether this player should discard cards before end this round. */
	int32 CardCountToDiscard() const
	{
		return FMath::Max(0, CardInstanceList.Num() - MaxCardCount);
	}

	void RegisterPlayersForRelation();

	bool GetPlayerRelationStatistic(int32 TargetPlayerId, FPlayerRelationStatistic** PlayerRelationStatistic);

	void RecordReceivedDamage(int32 SourcePlayerId, float Damage);

	void ModifyPlayerHealth(int32 TargetPlayerId, int32 NewHealth);

	/** Get all player relation statistic. */
	UFUNCTION(BlueprintCallable, Category="StandardPlayerState")
	TArray<FPlayerRelationStatistic> GetPlayerRelationStatisticList() const { return TArray<FPlayerRelationStatistic>(PlayerStatisticArray); }

	/** Get the Buff Queue of this player. */
	UBuffQueue* GetBuffQueue() const { return PlayerBuffQueue; }

	/**
	 * Get player card index that meet given class.
	 * @param CardClasses		Classes to filter player cards.
	 * @param CardIndex			Index of cards fit class filter.
	 * @return Count of cards in filter result
	 */
	int32 GetPlayerCardByClass(TArray<TSubclassOf<class ACardBase>> CardClasses, TArray<int32>& CardIndex);

	/**
	 * Get random cards for transfer purpose.
	 * @param CardCount			Card amount.
	 * @param CardIndexList		Index list of all selected cards.
	 * @return Whether get enough cards or not.
	 */
	bool GetRandomTransferCards(int32 CardCount, TArray<int32>& CardIndexList);

	ACardBase* GetCardByIndex(int32 CardIndex);

#pragma region CardUsableFilter
	/**
	 * Set card classes that allow player to use.
	 */
	void ApplyCardUsableFilterByClass(TArray<TSubclassOf<ACardBase>> CardClasses);

	void ApplyCardUsableFilterByUseMethod(ECardUseMethod NewUseMethod);

	/**
	 * Clear all filters and make all cards usable.
	 */
	void ClearCardUsableFilter();

	void MarkAllCardUnUsable();

	int32 GetUsableCardCount() const { return UsableCardIndex.Num(); }

	TArray<int32> GetAllUsableCardIndex() const { return UsableCardIndex; }
#pragma endregion

#pragma region SkipGamePhase
	int32 GetSkipGamePhaseFlags() const { return SkipGamePhaseFlags; }

	void MarkGamePhasesAsSkip(int32 GamePhaseFlags);
	void EraseGamePhasesFromSkip(int32 GamePhaseFlags);
#pragma endregion SkipGamePhase

#pragma region Ability
	void AddAbility(UAbilityBase* NewAbility);

	void RemoveAbility(FName AbilityToRemove);

	void UseAbility(int32 AbilitySlot);
#pragma endregion Ability

#pragma region DebugInterface
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCardCountChangeSignature, int32, CardCount);
	/** Triggered when player card count changed. */
	UPROPERTY(BlueprintAssignable, Category="StandardPlayerState")
	FOnPlayerCardCountChangeSignature OnPlayerCardCountChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRelationInfoChangedSignature);
	/** Triggered when player relation info changed. */
	UPROPERTY(BlueprintAssignable, Category="StandardPlayerState")
	FOnPlayerRelationInfoChangedSignature OnPlayerRelationInfoChanged;
#pragma endregion DebugInterface

protected:
	virtual void BeginPlay() override;

	void InitializeCardVariable();

	void RefreshCardInfoList();

	/**
	 * Validate all card instances owned by this player. In case there are some cards not be removed in time after
	 * card finished.
	 */
	void ValidateCardInstances();

	UFUNCTION()
	void OnRep_CardInfoList();

	UFUNCTION()
	void OnRep_UsableCardIndex();

	UFUNCTION()
	void OnRep_SkipGamePhaseFlags();

	UFUNCTION()
	void OnRep_MaxCardCount();

	UFUNCTION()
	void OnRep_MaxUseNum();

	UFUNCTION()
	void OnRep_UsedCardNum();

	UFUNCTION()
	void OnRep_CardGainPerRound();

	UFUNCTION()
	void OnCardFinished();

	UFUNCTION()
	void OnAbilityCooldownChanged(int32 AbilitySlot, int32 CurrentCooldown);

	UFUNCTION()
	void OnAbilityAvailabilityChanged(int32 AbilitySlot, bool NewAvailability);

	void PostCardFinished();

	void RagdollWaitingTick();

protected:

	int32 UsingCardIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_CardInfoList)
	TArray<FCardInstanceDisplayInfo> CardInfoList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_UsableCardIndex)
	TArray<int32> UsableCardIndex;

	/** The list of card instances. (Server Only) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState")
	TArray<ACardBase*> CardInstanceList;

	/** The maximum count of cards in player hand. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_MaxCardCount)
	int32 MaxCardCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_MaxUseNum)
	int32 MaxUseNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_UsedCardNum)
	int32 UsedCardNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_CardGainPerRound)
	int32 CardGainPerRounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	ECardSelectionPurpose CardSelectionPurpose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	bool bAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	TArray<FPlayerRelationStatistic> PlayerStatisticArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	bool bIsRagdoll;

	FTimerHandle RagdollWaitingTimerHandle;

	UPROPERTY(Transient)
	UBuffQueue* PlayerBuffQueue;

	UPROPERTY(ReplicatedUsing=OnRep_SkipGamePhaseFlags)
	int32 SkipGamePhaseFlags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState")
	TArray<UAbilityBase*> Abilities;
};
