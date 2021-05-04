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

	void SetStealCardData(int32 NewCardCount, FColor NewCardColor);

protected:
	UPROPERTY(BlueprintReadOnly, Category="StealCardBeam", Replicated)
	int32 CardCount;

	UPROPERTY(BlueprintReadOnly, Category="StealCardBeam", Replicated)
	FColor CardItemColor;
};
