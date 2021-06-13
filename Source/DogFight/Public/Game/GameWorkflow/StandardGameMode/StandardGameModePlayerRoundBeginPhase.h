// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"

#include "StandardGameModePhaseBase.h"
#include "StandardGameModePlayerRoundBeginPhase.generated.h"

/**
 * Game phase that check buffs on current player and initialize current player for action.
 */
UCLASS(Blueprintable)
class UStandardGameModePlayerRoundBeginPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()

public:
	void SetPhaseAfterRequestFinish(FName NewPhase);

protected:
	virtual bool StartPhase() override;

	virtual void EndPhase() override;

	virtual void FinishPhase() override;

	UFUNCTION()
	virtual void OnPlayerBuffQueueBeginRoundFinished();

	UFUNCTION()
	void OnRequestFinishRound();

protected:
	FName PhaseAfterRequestFinish;

	bool bRequestFinish;
};
