// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

UCLASS(Blueprintable)
class DOGFIGHTGAME_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** List of all weapon action data. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData, meta=(AllowedTypes="WeaponActionDataAsset"))
	TArray<FPrimaryAssetId> WeaponActionList;
};
