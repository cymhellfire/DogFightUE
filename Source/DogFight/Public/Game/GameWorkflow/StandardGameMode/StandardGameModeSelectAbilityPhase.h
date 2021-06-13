// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeSelectAbilityPhase.generated.h"

UCLASS()
class UStandardGameModeSelectAbilityPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnCandidateAbilitySelected(class AStandardPlayerState* PlayerState);

protected:

	int32 AbilitySelectingPlayerCount;
};
