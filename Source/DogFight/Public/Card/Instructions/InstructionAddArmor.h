// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/InstructionAddBuff.h"
#include "InstructionAddArmor.generated.h"

/**
 * Instruction that can add armor type buff to target.
 */
UCLASS()
class DOGFIGHT_API UInstructionAddArmor : public UInstructionAddBuff
{
	GENERATED_BODY()

protected:
	virtual void OnBuffCreated(IGameBuffInterface* BuffActor) override;
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty ArmorValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(Bitmask, BitmaskEnum = "EDogFightDamageCategory"))
	int32 AntiDamageCategories;

	/** Damage type list that are influenced by this armor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TArray<TSubclassOf<UDamageType>> DesiredDamageTypes;
};
