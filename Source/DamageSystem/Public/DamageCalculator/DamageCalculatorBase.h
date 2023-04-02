#pragma once

#include "DamageType/ExtendedDamageEvent.h"
#include "DamageCalculatorBase.generated.h"

class UExtendedDamageInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageEventOccured, UExtendedDamageInstance*, DamageInstance, const FExtendedDamageEvent&, DamageEvent);

UCLASS()
class DAMAGESYSTEM_API UDamageCalculatorBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="DamageCalculator")
	virtual void ApplyDamage(AActor* DamagedActor, float BaseDamage, UExtendedDamageInstance* DamageInstance, AActor* DamageCauser, AController* Instigator);

public:
	UPROPERTY(BlueprintAssignable, Category="DamageCalculator")
	FDamageEventOccured OnDamageEventOccured;
};
