// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Vfx/BeamVfx.h"
#include "StealCardBeamVfx.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStealCardBeamVfx : public ABeamVfx
{
	GENERATED_BODY()

public:

	void SetCardItemColor(FColor NewColor);

	UFUNCTION(BlueprintImplementableEvent, Category="StealCardBeam", meta=(DisplayName="OnCardItemColorSet"))
	void K2_OnCardItemColorSet(FColor NewColor);
};
