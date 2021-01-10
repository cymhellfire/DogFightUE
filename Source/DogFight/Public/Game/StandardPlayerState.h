// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "GameFramework/PlayerState.h"
#include "CustomizableCardTypes.h"
#include "StandardPlayerState.generated.h"

class ACardBase;

/**
 * Struct that handles the data describe relationship between players.
 */
USTRUCT(BlueprintType)
struct FPlayerRelationStatistic
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 PlayerId;

	UPROPERTY(VisibleAnywhere)
	bool IsAIPlayer;

	/** Represents Enemy if this value less than 0. Represents Ally when larger than 0. */
	UPROPERTY(VisibleAnywhere)
	int32 RelationPoint;

	UPROPERTY(VisibleAnywhere)
	int32 ReceiveDamage;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentHealth;
};

UENUM(BlueprintType)
enum class ECardSelectionPurpose : uint8
{
	CSP_Use			UMETA(DisplayName="Use"),
	CSP_Discard		UMETA(DisplayName="Discard"),
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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerCardInfoListChangedSignature);
	FPlayerCardInfoListChangedSignature OnPlayerCardInfoListChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUsingCardFinsishedSignature, bool, bPlayerRoundFinished);
	FUsingCardFinsishedSignature OnUsingCardFinished;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiscardCardFinishedSignature);
	FDiscardCardFinishedSignature OnDiscardCardFinished;

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

	FORCEINLINE bool CanUseCard() const { return UsedCardNum < MaxUseNum; }

	/** Initialize the player for his new round. */
	void InitializePlayerForNewRound();

	/** Get the number of cards can be gained in this round. */
	int32 GetCardGainNumByRound();

	FORCEINLINE TArray<FCardInstanceDisplayInfo> GetCardDisplayInfoList() const { return CardInfoList; }

	/** Get the card instance list. */
	FORCEINLINE TArray<ACardBase*> GetCardInstanceList() const { return CardInstanceList; }

	void SetAlive(bool bIsAlive);

	FORCEINLINE bool IsAlive() const { return bAlive; }

	void SetRagdollActive(bool bActive);

	/** Get the count of cards this player has. */
	FORCEINLINE int32 GetCurrentCardCount() const { return CardInstanceList.Num(); }

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
	TArray<FPlayerRelationStatistic> GetPlayerRelationStatisticList() const { return TArray<FPlayerRelationStatistic>(PlayerStatisticArray); }

protected:
	UFUNCTION()
	void OnRep_CardInfoList();

	UFUNCTION()
	void OnCardFinished();

	void PostCardFinished();

	void RagdollWaitingTick();

protected:

	int32 UsingCardIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_CardInfoList)
	TArray<FCardInstanceDisplayInfo> CardInfoList;

	/** The list of card instances. (Server Only) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState")
	TArray<ACardBase*> CardInstanceList;

	/** The maximum count of cards in player hand. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	int32 MaxCardCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	int32 MaxUseNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	int32 UsedCardNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
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
};
