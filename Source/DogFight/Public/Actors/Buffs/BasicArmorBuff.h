// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/BuffBase.h"
#include "DamageStructures.h"
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

	/** Initialize the extra armor add to target actor. Do this before ApplyBuff(). */
	virtual void InitializeArmor(int32 ArmorValue, int32 DamageCategories, TArray<TSubclassOf<UDamageType>> DesiredTypes);

protected:
	/** The extra armor add to target actor. */
	FActorArmor ExtraArmor;
};
