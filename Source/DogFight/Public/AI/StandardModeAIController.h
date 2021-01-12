// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "AI/DogFightAIController.h"
#include "Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Interfaces/GameTargetProviderInterface.h"

#include "StandardModeAIController.generated.h"

class AStandardModePlayerCharacter;

namespace EStandardModeAIControllerState
{
	extern DOGFIGHT_API const FName Idle;
	extern DOGFIGHT_API const FName UsingCard;
	extern DOGFIGHT_API const FName WaitingOrder;
	extern DOGFIGHT_API const FName Dead;
}

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

	AStandardModeAIController(const FObjectInitializer& ObjectInitializer);

	virtual void InitPlayerState() override;

	virtual void BeginDestroy() override;

	/** Enable the Free Movement of this AIController. */
	void EnableFreeMovement();

	void DisableFreeMovement();

	void StartAIRound();

	void StopAIRound();

	/** Use a random card. */
	void UseRandomCard();

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

	FORCEINLINE FName GetCurrentState() const { return CurrentState; }

	void SetState(FName NewState);

	AStandardModePlayerCharacter* GetCharacterPawn() const { return CharacterPawn; }

	/** Set target character pawn for next used card. */
	void SetTargetCharacter(AStandardModePlayerCharacter* NewTarget) { TargetCharacterPawn = NewTarget; };

#pragma region Interface
	virtual FCardTargetInfoAcquiredSignature& GetTargetInfoAcquiredDelegate() override { return OnCardTargetInfoAcquired; };

	virtual void RequestActorTarget() override;
	virtual void RequestPositionTarget() override;
	virtual void RequestDirectionTarget() override;

	virtual FCardInstructionTargetInfo RequestRandomActorTarget(bool bIgnoreSelf) override;
	virtual FCardInstructionTargetInfo RequestRandomPositionTarget() override;
	virtual FCardInstructionTargetInfo RequestRandomDirectionTarget() override;

	virtual APawn* GetActualPawn() override;
	virtual void BroadcastCardTargetingResult(FText CardName, FText TargetText, ECardInstructionTargetType TargetType) override;
#pragma endregion Interface

protected:
	virtual void BeginPlay() override;

	virtual void OnStateChanged(FName NewState);

	UFUNCTION()
	void OnCardFinished(bool bPlayerRoundFinished);

	UFUNCTION()
	void OnCharacterPawnDead();

	UFUNCTION()
	void OnHealthChanged(int32 NewHealth);

	AController* GetRandomTargetPlayer(bool bIgnoreSelf);

	void FilterForEnemyPlayer(TArray<struct FPlayerRelationStatistic>& ResultArray);

	void FilterForAllyPlayer(TArray<struct FPlayerRelationStatistic>& ResultArray);

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

	FName CurrentState;

	FCardTargetInfoAcquiredSignature OnCardTargetInfoAcquired;
};
