#pragma once

#include "CoreMinimal.h"
#include "DamageStructures.generated.h"

USTRUCT(BlueprintType)
struct FActorArmor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ActorArmor")
	int32 ArmorValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ActorArmor")
	TSubclassOf<UDamageType> AntiDamageType;

	/** Check if this armor can reduce specified damage value. */
	bool IsAgainstDamageType(TSubclassOf<UDamageType> TestDamageType) const
	{
		return AntiDamageType == TestDamageType;
	}
};
