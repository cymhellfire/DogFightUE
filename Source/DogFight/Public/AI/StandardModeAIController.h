// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "AIType.h"
#include "AI/DogFightAIController.h"
#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Actors/Interfaces/GameTargetProviderInterface.h"
#include "Card/GameCardTypes.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"

#include "StandardModeAIController.generated.h"

class AStandardModePlayerCharacter;

/**
 * AI Controller for StandardMode
 */
UCLASS()
class DOGFIGHT_API AStandardModeAIController : public ADogFightAIController, public IGameCardUserPlayerControllerInterface, public IGameTargetProviderInterface
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIPlayerDeadSignature, int32, PlayerId);
	FAIPlayerDeadSignature OnAIPlayerDead;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerHealthChangedSignature, int32, PlayerId, int32, NewHealth);
	FPlayerHealthChangedSignature OnPlayerHealthChanged;
	FPlayerHealthChangedSignature OnPlayerStrengthChanged;

	AStandardModeAIController(const FObjectInitializer& ObjectInitializer);

	virtual void InitPlayerState() override;

	virtual void BeginDestroy() override;

	/** Enable the Free Movement of this AIController. */
	void EnableFreeMovement();

	void DisableFreeMovement();

	void StartAIRound();

	void StopAIRound();

	void PrepareForUsingCard();

	/** Use a random card. */
	void UseRandomCard();

	/** Discard random cards by count. */
	void DiscardRandomCards(int32 Count);

	/**
	 * Use a card by giving specified category flags.
	 * @return Whether card found and used successfully.
	 * @param CategoryFlags		The category bitmask to filter cards.
	 */
	bool UseCardByCategoryFlags(int32 CategoryFlags);

	/**
	 * Find a target player character with given conditions.
	 * @return Character pawn of target player.
	 * @param TargetFlags		The filter condition for target player.
	 */
	AStandardModePlayerCharacter* AcquireTargetPlayerCharacter(int32 TargetFlags);

	FORCEINLINE EAIControllerState GetCurrentState() const { return CurrentState; }

	void SetState(EAIControllerState NewState);

	AStandardModePlayerCharacter* GetCharacterPawn() const { return CharacterPawn; }

	/** Set target character pawn for next used card. */
	void SetTargetCharacter(AStandardModePlayerCharacter* NewTarget) { TargetCharacterPawn = NewTarget; };

	bool HasUsableCard() const;

	bool UseResponseCard();

#pragma region Interface
	virtual FCardTargetInfoAcquiredSignature& GetTargetInfoAcquiredDelegate() override { return OnCardTargetInfoAcquired; };
	virtual FOnTargetActorSelectedSignature& GetOnTargetActorSelectedDelegate() override { return OnTargetActorSelected; }

	virtual void RequestActorTarget() override;
	virtual void RequestPositionTarget() override;
	virtual void RequestDirectionTarget() override;

	virtual FCardInstructionTargetInfo RequestRandomActorTarget(bool bIgnoreSelf) override;
	virtual FCardInstructionTargetInfo RequestRandomPositionTarget() override;
	virtual FCardInstructionTargetInfo RequestRandomDirectionTarget() override;

	virtual APawn* GetActualPawn() const override;
	virtual void BroadcastCardTargetingResult(FText CardName, FText TargetText, ECardInstructionTargetType TargetType) override;
#pragma endregion Interface

protected:
	virtual void BeginPlay() override;

	virtual void OnStateChanged(EAIControllerState NewState);

	void FinishMyRound();

	UFUNCTION()
	void OnCardFinished(bool bPlayerRoundFinished);

	UFUNCTION()
	void OnCharacterPawnDead();

	UFUNCTION()
	void OnHealthChanged(int32 NewHealth);

	UFUNCTION()
	void OnStrengthChanged(int32 NewStrength);

	AController* GetRandomTargetPlayer(bool bIgnoreSelf);

	void FilterAndSortForEnemyPlayer(TArray<struct FPlayerRelationStatistic>& ResultArray, int32& TopCount);

	void FilterAndSortForAllyPlayer(TArray<struct FPlayerRelationStatistic>& ResultArray, int32& TopCount);

	void FilterForHumanPlayer(TArray<struct FPlayerRelationStatistic>& ResultArray);

	void FilterForAIPlayer(TArray<struct FPlayerRelationStatistic>& ResultArray);

	void FilterForAlivePlayer(TArray<struct FPlayerRelationStatistic>& ResultArray);

	void FilterForDeadPlayer(TArray<struct FPlayerRelationStatistic>& ResultArray);

public:
	UPROPERTY(Category="AIController", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AStandardModePlayerCharacter> CharacterPawnClass;

	UPROPERTY(Category="AIController", EditDefaultsOnly, BlueprintReadOnly)
	float FreeMovementRadius;

	UPROPERTY(Category="AIController", EditDefaultsOnly, BlueprintReadOnly)
	UBehaviorTree* AIBehaviorTree;

protected:
	UPROPERTY(Category="AIController", VisibleAnywhere)
	AStandardModePlayerCharacter* CharacterPawn;

	UPROPERTY(VisibleAnywhere, Category="AIController")
	AStandardModePlayerCharacter* TargetCharacterPawn;

	EAIControllerState CurrentState;

	FCardTargetInfoAcquiredSignature OnCardTargetInfoAcquired;

	FOnTargetActorSelectedSignature OnTargetActorSelected;
};
