// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "InstructionReviveCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionReviveCharacter : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

protected:
	virtual bool HandleActorTarget(AActor* Target) override;
};
