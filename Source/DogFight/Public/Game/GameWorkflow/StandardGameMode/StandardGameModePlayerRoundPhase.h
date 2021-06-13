// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModePlayerRoundPhase.generated.h"

/**
 * Game phase that allow current player use cards.
 */
UCLASS()
class UStandardGameModePlayerRoundPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
public:
	void SetPhaseAfterRequestFinish(FName NewPhase);

protected:
	virtual bool StartPhase() override;

	virtual void EndPhase() override;

	virtual void FinishPhase() override;

	UFUNCTION()
	void OnRequestFinishRound();

	UFUNCTION()
	void OnPlayerUsingCardFinished(bool bShouldEndRound);

protected:
	FName PhaseAfterRequestFinish;

	bool bRequestFinish;
};
