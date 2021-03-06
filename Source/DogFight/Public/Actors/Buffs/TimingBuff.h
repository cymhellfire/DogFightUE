// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/BuffBase.h"
#include "TimingBuff.generated.h"

/**
 * Buff that take effect only after given duration. 
 */
UCLASS()
class DOGFIGHT_API ATimingBuff : public ABuffBase
{
	GENERATED_BODY()

public:
	ATimingBuff();

protected:
	virtual void RemoveBuff() override;

	/** The function handle all buff operations. */
	virtual void TimerExpired();

protected:
	/** The Vfx plays when buff take action. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TimingBuff")
	TSubclassOf<AActor> ActionVfxClass;

	/** Override end timer duration when this buff is successfully triggered. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TimingBuff")
	float TriggeredEndTimerDuration;
};
