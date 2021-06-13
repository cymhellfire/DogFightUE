// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModePlayerRoundEndPhase.generated.h"

UCLASS()
class UStandardGameModePlayerRoundEndPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnPlayerBuffQueueEndRoundFinished();
};
