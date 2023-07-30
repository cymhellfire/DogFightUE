// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponActionDataAsset.generated.h"

/**
 * Data asset that defines various weapon actions.
 */
UCLASS(Blueprintable)
class DOGFIGHTGAME_API UWeaponActionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	bool bNeedTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	TSoftObjectPtr<UAnimMontage> AnimMontage;
};
