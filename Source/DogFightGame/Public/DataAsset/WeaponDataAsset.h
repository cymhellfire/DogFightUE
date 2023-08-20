// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

class UWeaponActionDataAsset;

UCLASS(Blueprintable)
class DOGFIGHTGAME_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	FText WeaponName;

	/** List of all weapon action data. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	TArray<UWeaponActionDataAsset*> WeaponActionList;

	/** Default action when equipped action. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	UWeaponActionDataAsset* DefaultAction;
};
