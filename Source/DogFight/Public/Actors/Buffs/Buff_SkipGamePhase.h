// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/BuffBase.h"
#include "Buff_SkipGamePhase.generated.h"

/**
 * A buff can skip specified game phases of target player's round.
 */
UCLASS()
class DOGFIGHT_API ABuff_SkipGamePhase : public ABuffBase
{
	GENERATED_BODY()
public:
	ABuff_SkipGamePhase();

protected:
	virtual void ApplyBuff() override;
	virtual void RemoveBuff() override;

	virtual void OnTargetPlayerRoundBegin() override;

	virtual TArray<int32> SetupCustomLifetime(int32 TimelineFactor) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff", meta=(Bitmask, BitmaskEnum="ESkippableGamePhase"))
	int32 SkipGamePhase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	FLocalizedString TriggeredFloatingText;
};
