// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Vfx/VfxBase.h"
#include "BeamVfx.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ABeamVfx : public AVfxBase
{
	GENERATED_BODY()

public:
	void SetBeamTargetActor(AActor* BeamTargetActor);

	UFUNCTION(BlueprintImplementableEvent, Category="BeamVfx", meta=(DisplayName="OnBeamTargetActorSet"))
	void K2_OnBeamTargetActorSet(AActor* BeamTargetActor);
};
