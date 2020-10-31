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

	AStandardPlayerState(const FObjectInitializer& ObjectInitializer);

	virtual void OnRep_PlayerName() override;

	void AddCard(ACardBase* Card);

	UFUNCTION(Server, Reliable)
	void CmdUseCardByIndex(int32 Index);

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

	/** Get the count of cards this player has. */
	FORCEINLINE int32 GetCurrentCardCount() const { return CardInstanceList.Num(); }

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

protected:

	int32 UsingCardIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", ReplicatedUsing=OnRep_CardInfoList)
	TArray<FCardInstanceDisplayInfo> CardInfoList;

	/** The list of card instances. (Server Only) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState")
	TArray<ACardBase*> CardInstanceList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	int32 MaxUseNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	int32 UsedCardNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	int32 CardGainPerRounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	bool bAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardPlayerState", Replicated)
	TArray<FPlayerRelationStatistic> PlayerStatisticArray;
};
