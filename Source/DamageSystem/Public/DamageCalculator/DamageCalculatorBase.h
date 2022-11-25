#pragma once

#include "DamageCalculatorBase.generated.h"

class UExtendedDamageInstance;

UCLASS()
class DAMAGESYSTEM_API UDamageCalculatorBase : public UObject
{
	GENERATED_BODY()
public:

	virtual void ApplyDamage(AActor* DamagedActor, float BaseDamage, FName DamageTypeName, AActor* DamageCauser, AController* Instigator);

	UExtendedDamageInstance* GetDamageInstanceByName(FName InName) const;


protected:
	UPROPERTY(Transient)
	TMap<FName, UExtendedDamageInstance*> DamageInstanceTable;
};
