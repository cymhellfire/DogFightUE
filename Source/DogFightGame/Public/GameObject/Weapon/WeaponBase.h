// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCommon.h"
#include "UObject/Object.h"
#include "WeaponBase.generated.h"

class UWeaponBase;
class IActionCharacterInterface;
class UWeaponActionBase;
class UWeaponDataAsset;
class UWeaponActionDataAsset;

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
class DOGFIGHTGAME_API UWeaponBase : public UObject
{
	GENERATED_BODY()
	friend class UWeaponActionTransitionBase;
public:
	UWeaponBase();

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

protected:
	virtual void PerformAction(UWeaponActionBase* InAction, const FWeaponActionTarget& InTarget = nullptr);

	void OnActionFinished(UWeaponActionBase* InAction);

	void CheckInputQueue();

public:
	FWeaponBaseEvent OnWeaponInputFinished;

protected:
	UPROPERTY(Transient)
	TArray<UWeaponActionBase*> WeaponActions;

	UPROPERTY(Transient)
	UWeaponActionBase* CurrentAction;

	UPROPERTY(Transient)
	UWeaponActionBase* DefaultAction;

	uint8 bFiredInputFinished:1;

	IActionCharacterInterface* OwnerCharacter = nullptr;

	TArray<FWeaponActionInfo> InputQueue;
};
