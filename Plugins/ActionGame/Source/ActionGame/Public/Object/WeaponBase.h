// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Common/WeaponCommon.h"
#include "UObject/Object.h"
#include "WeaponBase.generated.h"

class UWeaponBase;
class IActionCharacterInterface;
class UWeaponActionBase;
class UWeaponDataAsset;
class UWeaponActionDataAsset;
class AWeaponModelBase;

struct FWeaponActionInfo
{
	FWeaponActionInfo()
		:Input(EWeaponActionInput::WAI_None)
		,Target(FWeaponActionTarget(nullptr))
	{}

	FWeaponActionInfo(EWeaponActionInput InInput, const FWeaponActionTarget& InTarget)
		:Input(InInput)
		,Target(InTarget)
	{}

	bool IsValid() const
	{
		return Input != EWeaponActionInput::WAI_None;
	}

	EWeaponActionInput Input;
	FWeaponActionTarget Target;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponBaseEvent, UWeaponBase*);

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UWeaponBase : public UObject
{
	GENERATED_BODY()
	friend class UWeaponActionTransitionBase;
public:
	UWeaponBase();

	virtual void BeginDestroy() override;

	void SetOwner(IActionCharacterInterface* InOwner)
	{
		OwnerCharacter = InOwner;
	}

	void InitWithWeaponData(UWeaponDataAsset* InWeaponData);

	virtual UWeaponActionBase* AddWeaponAction(UWeaponActionDataAsset* InActionData);

	virtual void EnqueueInput(EWeaponActionInput NewInput, const FWeaponActionTarget& InTarget = nullptr);

	virtual FWeaponActionInfo DequeueInput();

	virtual void StartInputQueue();

	virtual void ConsumeInput();

	/**
	 * Reset this weapon the default action and clear the input queue.
	 */
	void ResetWeapon();

	/**
	 * Change the attack detecting state of this weapon.
	 * @param bEnable Whether to enable attack detecting.
	 */
	virtual void SetAttackDetectEnable(bool bEnable);

protected:
	void SpawnWeaponActor();
	void DestroyWeaponActor();

	virtual void PerformAction(UWeaponActionBase* InAction, const FWeaponActionTarget& InTarget = nullptr);

	void OnActionFinished(UWeaponActionBase* InAction);

	void CheckInputQueue();

	virtual void OnHitTarget(AActor* TargetActor, UPrimitiveComponent* TargetComponent, const FHitResult& HitResult);

public:
	FWeaponBaseEvent OnWeaponInputFinished;

protected:
	UPROPERTY(Transient)
	TArray<UWeaponActionBase*> WeaponActions;

	UPROPERTY(Transient)
	UWeaponActionBase* CurrentAction;

	UPROPERTY(Transient)
	UWeaponActionBase* DefaultAction;

	uint8 bFiredInputFinished: 1;

	FName ParentSocket;

	TSoftClassPtr<AWeaponModelBase> WeaponModelClass;
	TWeakObjectPtr<AWeaponModelBase> WeaponModel;

	IActionCharacterInterface* OwnerCharacter = nullptr;

	TArray<FWeaponActionInfo> InputQueue;
};
