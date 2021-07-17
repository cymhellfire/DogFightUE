// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "VfxBase.h"
#include "MoveToTargetVfx.generated.h"

class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AMoveToTargetVfx : public AVfxBase
{
	GENERATED_BODY()

public:
	AMoveToTargetVfx();

	void SetTargetLocation(FVector NewTarget);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vfx")
	FVector MoveTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vfx")
	UNiagaraComponent* NiagaraParticleSystem;
};
