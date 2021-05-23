// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "Common/LocalizedString.h"
#include "AbilityCommon.h"
#include "AbilityDisplayInfo.generated.h"

USTRUCT(BlueprintType)
struct FAbilityDisplayInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityDisplayInfo")
	FLocalizedString AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityDisplayInfo")
	FLocalizedString AbilityDesc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityDisplayInfo")
	UTexture2D* AbilityIcon;

	UPROPERTY(BlueprintReadOnly)
	EAbilityCastType CastType;

	UPROPERTY(BlueprintReadOnly)
	int32 AbilityCooldown;
};
