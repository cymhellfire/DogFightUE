// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

class AWeaponModelBase;
class UWeaponBase;
class UWeaponActionDataAsset;

UCLASS(Blueprintable)
class ACTIONGAME_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	FText WeaponName;

	/** Name of parent slot when this weapon is equipped. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	FName ParentSocket;

	/** Class of weapon object. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	TSoftClassPtr<UWeaponBase> WeaponClass;

	/** Class of weapon actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	TSoftClassPtr<AWeaponModelBase> WeaponModelClass;

	/** List of all weapon action data. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	TArray<UWeaponActionDataAsset*> WeaponActionList;

	/** Default action when equipped action. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WeaponData)
	UWeaponActionDataAsset* DefaultAction;
};
