// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCommon.h"
#include "UObject/Object.h"
#include "WeaponActionBase.generated.h"

class UWeaponBase;
class UWeaponActionDataAsset;
class UWeaponActionTransitionBase;
class IActionCharacterInterface;

/**
 * Base class of all weapon actions.
 */
UCLASS()
class DOGFIGHTGAME_API UWeaponActionBase : public UObject
{
	GENERATED_BODY()

protected:
	enum class EDistanceCheckResult
	{
		Invalid,
		OutOfRange,
		InRange,
	};

public:
	UWeaponActionBase();

	virtual void SetOwnerWeapon(UWeaponBase* InOwner)
	{
		OwnerWeapon = InOwner;
	}

	virtual void InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* InOwner);

	virtual void AddTransition(EWeaponActionInput InInput, UWeaponActionBase* InAction);

	virtual void SetActionTarget(const FWeaponActionTarget& InTarget);

	virtual void Execute();

	virtual void PerformAction();

	virtual void ConsumeInput();

protected:
	/**
	 * @brief Check the distance between performer and target.
	 * @return Whether target is in range.
	 */
	virtual EDistanceCheckResult CheckDistance();

	virtual void GoToTarget();

	virtual void OnReachActionDistance();

	/**
	 * @brief Play the montage asset.
	 * @return Duration of played montage.
	 */
	virtual float PlayActionMontage();

	UFUNCTION()
	void OnActionMontageFinished();

	void OnActionFinished();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	FText ActionName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	FText ActionDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	UAnimMontage* ActionMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	bool bNeedTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	float ActionRange;

protected:
	UPROPERTY(Transient)
	UWeaponBase* OwnerWeapon;

	UPROPERTY(Transient)
	TMap<EWeaponActionInput, UWeaponActionTransitionBase*> TransitionMap;

	IActionCharacterInterface* Performer;
	TOptional<FWeaponActionTarget> ActionTarget;

	FTimerHandle ActionTimerHandler;
};
