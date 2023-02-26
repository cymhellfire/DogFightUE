#pragma once

#include "DamageCalculatorBase.generated.h"

class UExtendedDamageInstance;

UCLASS()
class DAMAGESYSTEM_API UDamageCalculatorBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="DamageCalculator")
	virtual void ApplyDamage(AActor* DamagedActor, float BaseDamage, UExtendedDamageInstance* DamageInstance, AActor* DamageCauser, AController* Instigator);

};
