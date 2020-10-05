// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "GameFramework/PlayerState.h"
#include "CustomizableCardTypes.h"
#include "StandardPlayerState.generated.h"

class ACardBase;

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

	void SetAlive(bool bIsAlive);

	FORCEINLINE bool IsAlive() const { return bAlive; }

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
};
