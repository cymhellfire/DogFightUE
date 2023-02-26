#pragma once

#include "Warhead.generated.h"

/**
 * Warhead is a data structure hold game effect and damage type together.
 */
USTRUCT(BlueprintType)
struct DOGFIGHTGAME_API FWarhead
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead")
	FString WarheadName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead|Effect")
	int32 GameEffectId = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead|Effect")
	bool bUseProjectileRotation = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead|Damage")
	FName DamageTypeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead|Damage")
	int32 DamageId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Warhead|Damage")
	float DamageRadius = 0.f;
};
