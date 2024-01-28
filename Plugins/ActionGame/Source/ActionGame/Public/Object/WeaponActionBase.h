// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Common/WeaponCommon.h"
#include "UObject/Object.h"
#include "WeaponActionBase.generated.h"

class UWeaponBase;
class UWeaponActionBase;
class UWeaponActionDataAsset;
class UWeaponActionTransitionBase;
class IActionCharacterInterface;

DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponActionBaseEvent, UWeaponActionBase*);

/**
 * Base class of all weapon actions.
 */
UCLASS()
class ACTIONGAME_API UWeaponActionBase : public UObject
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

	virtual bool InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* InOwner);

	virtual void AddTransition(EWeaponActionInput InInput, UWeaponActionBase* InAction);

	virtual void SetActionTarget(const FWeaponActionTarget& InTarget) {}

	virtual bool Execute();

	virtual void PerformAction();

	virtual UWeaponActionTransitionBase* GetTransitionByInput(EWeaponActionInput Input) const;

protected:
	/**
	 * @brief Play the montage asset.
	 * @return Duration of played montage.
	 */
	virtual float PlayActionMontage();

	UFUNCTION()
	void OnActionMontageFinished();

	void FinishAction();

	/** Bind all animation notifies in action montage. */
	virtual void BindAnimNotifies();

	/** Unbind all animation notifies in action montage. */
	virtual void UnbindAnimNotifies();

	virtual void OnAttackDetectStateChanged(bool bEnable);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	FText ActionName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	FText ActionDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	UAnimMontage* ActionMontage;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	// bool bNeedTarget;

	FWeaponActionBaseEvent OnActionFinished;

protected:
	UPROPERTY(Transient)
	UWeaponBase* OwnerWeapon;

	UPROPERTY(Transient)
	TMap<EWeaponActionInput, UWeaponActionTransitionBase*> TransitionMap;

	IActionCharacterInterface* Performer;

	FTimerHandle ActionTimerHandler;
};
