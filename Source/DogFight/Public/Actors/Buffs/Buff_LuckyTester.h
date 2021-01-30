// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/Buff_FollowCurrentPlayer.h"
#include "Buff_LuckyTester.generated.h"

class ATriggerableProjectile;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ABuff_LuckyTester : public ABuff_FollowCurrentPlayer
{
	GENERATED_BODY()

protected:
	virtual void ApplyBuff() override;

	virtual void RemoveBuff() override;

	virtual void OnTargetPlayerRoundBegin() override;

	void DoTest();

	void TransferTesterToTarget();

	UFUNCTION()
	void OnTesterActorDead(AActor* Tester);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuckyTester")
	TSubclassOf<ATriggerableProjectile> TesterActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuckyTester", meta=(ClampMin="0", ClampMax="1"))
	float FailureRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuckyTester")
	FLocalizedString TestPassedText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuckyTester")
	FLocalizedString TestFailedText;

	/** The start offset of tester actor every time transfer to current player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuckyTester")
	FVector TransferOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuckyTester")
	float RandomOffsetRadius;

	ATriggerableProjectile* TesterActor;
};
