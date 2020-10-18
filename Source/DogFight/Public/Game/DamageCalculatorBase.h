// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageCalculatorBase.generated.h"

/**
 * Damage calculator class.
 */
UCLASS()
class DOGFIGHT_API UDamageCalculatorBase : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Calculate the actual damage apply to the target actor.
	 * @param DamageTaker		The actor takes the damage.
	 * @param Damage			How much damage to apply.
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 */
	virtual float CalculateActualDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};
