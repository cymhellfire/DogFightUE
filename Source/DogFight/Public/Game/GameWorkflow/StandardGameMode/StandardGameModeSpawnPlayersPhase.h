// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeSpawnPlayersPhase.generated.h"

UCLASS()
class UStandardGameModeSpawnPlayersPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
protected:
	virtual bool StartPhase() override;

	virtual void FinishPhase() override;

	void StartSpawnAI();

	UFUNCTION()
	void SpawnPlayerTick();

	UFUNCTION()
	void SpawnAITick();

protected:
	int32 CurrentSpawnPlayerIndex;
	int32 AICount;

	FTimerHandle SpawnPlayerTimerHandle;
};
