// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "InstructionStealCard.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionStealCard : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

protected:
	virtual bool HandleActorTarget(AActor* Target) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty CardCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TSubclassOf<class AStealCardBeamVfx> BeamVfxClass;
};
