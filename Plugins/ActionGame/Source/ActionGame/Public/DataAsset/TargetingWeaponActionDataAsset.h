// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActionDataAsset.h"
#include "Object/TargetingWeaponAction.h"
#include "TargetingWeaponActionDataAsset.generated.h"

/**
 * Data asset for targeting weapon action.
 */
UCLASS()
class ACTIONGAME_API UTargetingWeaponActionDataAsset : public UWeaponActionDataAsset
{
	GENERATED_BODY()

public:
	UTargetingWeaponActionDataAsset()
	{
		bWarpingToTarget = false;
		Range = 0.f;
		ActionClass = UTargetingWeaponAction::StaticClass();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	bool bWarpingToTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction", meta=(EditCondition="bWarpingToTarget==true", EditConditionHides))
	FName WarpingTargetName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	float Range;
};
