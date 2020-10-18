// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/BuffBase.h"
#include "BasicArmorBuff.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ABasicArmorBuff : public ABuffBase
{
	GENERATED_BODY()

public:
	virtual void ApplyBuff() override;
	virtual void RemoveBuff() override;

public:
	/** The physical armor changed amount. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	int32 PhysicalArmor;

	/** The magical armor changed amount. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	int32 MagicalArmor;
};
