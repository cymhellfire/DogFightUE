// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"

#include "DogFightGameStateBase.h"
#include "StandardMiniMapCapture.h"
#include "StandardGameState.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardGameState : public ADogFightGameStateBase
{
	GENERATED_UCLASS_BODY()

public:
	/* Mini Map Capture Camera */
	TWeakObjectPtr<AStandardMiniMapCapture> MiniMapCamera;

	UPROPERTY(Category=Map, VisibleAnywhere, BlueprintReadOnly)
	/* World bounds for Mini Map and Camera movement. */
	FBox WorldBounds;

	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;
};
