// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModeTimedPhase.h"
#include "StandardGameModeFreeMovingPhase.generated.h"

UCLASS()
class UStandardGameModeFreeMovingPhase : public UStandardGameModeTimedPhase
{
	GENERATED_BODY()
protected:
	virtual bool StartPhase() override;

	virtual void EndPhase() override;
};
