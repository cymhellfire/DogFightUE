// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Weapon/WeaponCommon.h"
#include "WeaponActionDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FWeaponActionTransitionSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponActionTransitionSettings")
	EWeaponActionInput InputType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponActionTransitionSettings")
	UWeaponActionDataAsset* WeaponAction;
};

/**
 * Data asset that defines various weapon actions.
 */
UCLASS(Blueprintable)
class DOGFIGHTGAME_API UWeaponActionDataAsset : public UObject
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
	bool bWarpingToTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction", meta=(EditCondition="bWarpingToTarget==true", EditConditionHides))
	FName WarpingTargetName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	TSoftObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	TArray<FWeaponActionTransitionSettings> TransitionSettings;
};
