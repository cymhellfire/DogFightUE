// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeDiscardCardsPhase.generated.h"

/**
 * Game phase that let current player discard cards exceeds the maximum capacity.
 */
UCLASS()
class UStandardGameModeDiscardCardsPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnPlayerDiscardCardFinished();
};
