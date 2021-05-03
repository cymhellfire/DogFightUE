#pragma once

#include "CoreMinimal.h"
#include "WeaponDisplayRelative.generated.h"

USTRUCT(BlueprintType)
struct FWeaponActionDisplayInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="WeaponActionDisplayInfo")
	FString ActionName;

	UPROPERTY(BlueprintReadOnly, Category="WeaponActionDisplayInfo")
	FString ActionDescription;

	UPROPERTY(BlueprintReadOnly, Category="WeaponActionDisplayInfo")
	FString ActionDamageString;

	UPROPERTY(BlueprintReadOnly, Category="WeaponActionDisplayInfo")
	FString ActionRangeString;
};
