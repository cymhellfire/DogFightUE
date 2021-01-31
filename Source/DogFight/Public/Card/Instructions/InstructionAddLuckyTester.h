// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/InstructionAddBuff.h"
#include "InstructionAddLuckyTester.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionAddLuckyTester : public UInstructionAddBuff
{
	GENERATED_BODY()

protected:
	virtual void OnBuffCreated(IGameBuffInterface* BuffActor) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableFloatProperty TestFailRadio;
};
