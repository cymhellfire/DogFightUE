// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeCheckGameEndPhase.generated.h"

/**
 * Game phase that check if current game is finished.
 */
UCLASS()
class UStandardGameModeCheckGameEndPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()

public:
	void SetEndGamePhase(FName PhaseName);

protected:
	virtual bool StartPhase() override;

	virtual void FinishPhase() override;

protected:
	FName EndGamePhaseName;
};
