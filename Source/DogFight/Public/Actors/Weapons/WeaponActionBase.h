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
	/** Prepare for playing action montage. */
	virtual void PrepareActionMontage();

	virtual void PlayActionMontage(UAnimMontage* MontageToPlay);

	UFUNCTION()
	void OnReachedActionDistance(AActor* Carrier);

	UFUNCTION()
	void OnActionMontageFinished();

	UFUNCTION()
	void OnResponseCardSelected();

	UFUNCTION()
	void OnHitDetectSwitched(class UAnimNotify_SwitchHitDetect* Notify, bool bTurnOn, float DamageRatio, FName ParentSocketName);

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

	/** Card classes that can response to this action. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponAction")
	TArray<TSubclassOf<class ACardBase>> ResponseCardClasses;

protected:
	UWeaponBase* OwnerWeapon;

	FTimerHandle ActionTimerHandle;
};
