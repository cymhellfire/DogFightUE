// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCommon.h"
#include "WeaponActionBase.generated.h"

class UWeaponBase;

/**
* Transition between two weapon action.
*/
USTRUCT(BlueprintType)
struct FWeaponActionTransition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ActionTransition")
	FName TargetActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ActionTransition")
	EWeaponActionInput TransitionInput;
};

/**
 * 
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UWeaponActionBase : public UObject
{
	GENERATED_BODY()

public:
	UWeaponActionBase();

	virtual void EnterAction();

	virtual FName GetNextActionWithInput(EWeaponActionInput NewInput);

	void SetOwnerWeapon(UWeaponBase* NewWeapon);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponActionFinished);
	FOnWeaponActionFinished OnWeaponActionFinished;

protected:
	UFUNCTION()
	void OnReachedActionDistance(AActor* Carrier);

	UFUNCTION()
	void OnActionMontageFinished();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponAction")
	FName WeaponActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponAction")
	UAnimMontage* ActionMontage;

	/** The maximum distance from target that allow this action start. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponAction")
	float ActionDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponAction")
	TArray<FWeaponActionTransition> ActionTransitions;

protected:
	UWeaponBase* OwnerWeapon;

	FTimerHandle ActionTimerHandle;
};
