// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActionBase.h"
#include "TargetingWeaponAction.generated.h"

/**
 * Base class for weapon actions that require a target to perform at.
 */
UCLASS()
class ACTIONGAME_API UTargetingWeaponAction : public UWeaponActionBase
{
	GENERATED_BODY()

public:
	virtual bool InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* InOwner) override;

	virtual void SetActionTarget(const FWeaponActionTarget& InTarget) override;

	virtual bool Execute() override;

protected:
	/**
	 * @brief Check the distance between performer and target.
	 * @return Whether target is in range.
	 */
	virtual EDistanceCheckResult CheckDistance();

	virtual void UpdateCollisionCorrectDistance();

	virtual void GoToTarget();

	virtual void OnReachActionDistance();

	virtual float PlayActionMontage() override;

	FORCEINLINE float GetFinalActionDistance() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	bool bWarpingToTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	float ActionRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction", meta=(EditCondition="bWarpingToTarget==true", EditConditionHides))
	FName WarpingTargetName;

protected:
	float CollisionCorrectDistance;

	TOptional<FWeaponActionTarget> ActionTarget;
};
