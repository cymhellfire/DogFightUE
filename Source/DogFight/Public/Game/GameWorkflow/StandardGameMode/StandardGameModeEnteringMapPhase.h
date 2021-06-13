// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeEnteringMapPhase.generated.h"

/**
 * Game phase that waiting for all players loading map.
 */
UCLASS()
class UStandardGameModeEnteringMapPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnPlayerReadyForGame();

protected:
	int32 DesiredPlayerCount;

	int32 ReadyCount;
};
