// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "AI/DogFightAIController.h"
#include "GameCardUserPlayerControllerInterface.h"
#include "GameTargetProviderInterface.h"

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

	AStandardModeAIController(const FObjectInitializer& ObjectInitializer);

	virtual void InitPlayerState() override;

	virtual void BeginDestroy() override;

	/** Enable the Free Movement of this AIController. */
	void EnableFreeMovement();

	void DisableFreeMovement();

	void StartAIRound();

	void StopAIRound();

	void UseRandomCard();

	FORCEINLINE FName GetCurrentState() const { return CurrentState; }

	void SetState(FName NewState);

#pragma region Interface
	virtual FCardTargetInfoAcquiredSignature& GetTargetInfoAcquiredDelegate() override { return OnCardTargetInfoAcquired; };

	virtual void RequestActorTarget() override;
	virtual void RequestPositionTarget() override;
	virtual void RequestDirectionTarget() override;

	virtual APawn* GetActualPawn() override;
#pragma endregion Interface

protected:
	virtual void BeginPlay() override;

	virtual void OnStateChanged(FName NewState);

	UFUNCTION()
	void OnCardFinished(bool bPlayerRoundFinished);

	UFUNCTION()
	void OnCharacterPawnDead();

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

	FName CurrentState;

	FCardTargetInfoAcquiredSignature OnCardTargetInfoAcquired;
};
