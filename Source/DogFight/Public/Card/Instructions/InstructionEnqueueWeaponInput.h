// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "InstructionEnqueueWeaponInput.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionEnqueueWeaponInput : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

protected:
	virtual bool HandleActorTarget(AActor* Target) override;

	UFUNCTION()
	void OnActionFinished(AActor* Carrier);

protected:

};
