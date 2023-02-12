#pragma once

#include "WarheadBase.generated.h"

/**
 * Warhead is a data structure hold game effect and damage type together.
 */
UCLASS(Blueprintable)
class DOGFIGHTGAME_API UWarheadBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead")
	FString WarheadName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead|Effect")
	int32 GameEffectId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead|Effect")
	bool bUseProjectileRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead")
	FName DamageTypeName;
};
