// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeTimedPhase.generated.h"

UCLASS()
class UStandardGameModeTimedPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()

public:

	void SetPhaseDuration(int32 NewDuration);

protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnTimerTick();

	virtual void OnTimerExpired();

protected:
	int32 PhaseDuration;

	FTimerHandle PhaseTimerHandle;

	class AStandardGameState* TargetGameState;
};
