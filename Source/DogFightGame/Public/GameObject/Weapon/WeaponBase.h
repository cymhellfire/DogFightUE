// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCommon.h"
#include "UObject/Object.h"
#include "WeaponBase.generated.h"

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

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UWeaponBase : public UObject
{
	GENERATED_BODY()
	friend class UWeaponActionTransitionBase;
public:

	void SetOwner(IActionCharacterInterface* InOwner)
	{
		OwnerCharacter = InOwner;
	}

	void InitWithWeaponData(UWeaponDataAsset* InWeaponData);

	virtual UWeaponActionBase* AddWeaponAction(UWeaponActionDataAsset* InActionData);

	virtual void EnqueueInput(EWeaponActionInput NewInput, const FWeaponActionTarget& InTarget = nullptr);

	virtual FWeaponActionInfo DequeueInput();

	virtual void StartInputQueue();

protected:
	virtual void PerformAction(UWeaponActionBase* InAction, const FWeaponActionTarget& InTarget = nullptr);

protected:
	UPROPERTY(Transient)
	TArray<UWeaponActionBase*> WeaponActions;
	
	UPROPERTY(Transient)
	UWeaponActionBase* CurrentAction;

	IActionCharacterInterface* OwnerCharacter = nullptr;

	TArray<FWeaponActionInfo> InputQueue;
};
