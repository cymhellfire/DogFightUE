#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/WeaponCommon.h"
#include "WeaponCarrierInterface.generated.h"

class UWeaponBase;

UINTERFACE(MinimalAPI, Blueprintable, meta=(CannotImplementInterfaceInBlueprint))
class UWeaponCarrierInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedSignature, AActor*, CarrierActor);

/**
 * Interface that support equip weapon.
 */
class IWeaponCarrierInterface
{
	GENERATED_BODY()

public:

	/** Get the weapon currently equipped. */
	virtual UWeaponBase* GetCurrentWeapon() { return nullptr; }

	/** Get the type of current weapon. */
	UFUNCTION(BlueprintCallable, Category="WeaponCarrierInterface")
	virtual EWeaponType GetCurrentWeaponType() = 0;

	/** Equip given weapon. */
	virtual void EquipWeapon(UWeaponBase* NewWeapon) {}

	/** UnEquip current weapon. */
	virtual void UnEquipWeapon() {}

	/** Enqueue new input to equipping weapon. */
	virtual void EnqueueInput(EWeaponActionInput NewInput) {}

	/** Get the delegate of weapon equipped event. */
	virtual FOnWeaponEquippedSignature& GetWeaponEquippedEvent() = 0;
};
