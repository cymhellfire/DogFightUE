// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/InstructionAddBuff.h"
#include "InstructionAddChaos.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionAddChaos : public UInstructionAddBuff
{
	GENERATED_BODY()

protected:
	virtual void OnBuffCreated(IGameBuffInterface* BuffActor) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableFloatProperty ChaosPossibility;
};
