// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "TimingBuff.h"
#include "Buff_IncredibleSurvive.generated.h"

/**
 * Lower target health and recover when buff normally ended.
 */
UCLASS()
class DOGFIGHT_API ABuff_IncredibleSurvive : public ATimingBuff
{
	GENERATED_BODY()
public:
	ABuff_IncredibleSurvive();

protected:
	virtual void ApplyBuff() override;
	virtual void TimerExpired() override;

protected:
	/** The health percentage applied to target actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff", meta=(ClampMin="0.05", ClampMax="1"))
	float HealthPercentage;

	/** Target actor restore health to this percentage value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff", meta=(ClampMin="0.1", ClampMax="1"))
	float RecoverPercentage;
};
