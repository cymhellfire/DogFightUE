// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Game/DamageCalculatorBase.h"
#include "StandardModeDamageCalculator.generated.h"

class UReceiveDamageComponent;

/**
 * Damage Calculator for Standard Game Mode
 */
UCLASS()
class DOGFIGHT_API UStandardModeDamageCalculator : public UDamageCalculatorBase
{
	GENERATED_BODY()

public:
	virtual float CalculateActualDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent,	AController* EventInstigator, AActor* DamageCauser) override;

protected:
	float HandleActorArmor(const UReceiveDamageComponent* ReceiveComponent, float Damage, int32 DamageCategory, FDamageEvent const& DamageEvent);
};
