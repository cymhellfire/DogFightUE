// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Common/WeaponCommon.h"
#include "UObject/Object.h"
#include "WeaponActionTransitionBase.generated.h"

class UWeaponBase;
class UWeaponActionBase;

/**
 * Transition behavior between weapon actions.
 */
UCLASS(BlueprintType)
class ACTIONGAMEWEAPON_API UWeaponActionTransitionBase : public UObject
{
	GENERATED_BODY()

public:
	void InitTransition(UWeaponBase* InOwner, EWeaponActionInput InInput, UWeaponActionBase* InAction);

	void DoTransition(const FWeaponActionTarget& InTarget = nullptr);

private:
	EWeaponActionInput KeyInput;

	UPROPERTY(Transient)
	UWeaponBase* OwnerWeapon;

	UPROPERTY(Transient)
	UWeaponActionBase* TargetAction;
};
