#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/WeaponCommon.h"
#include "Actors/Weapons/WeaponDisplayRelative.h"
#include "WeaponCarrierInterface.generated.h"

class UWeaponBase;

UINTERFACE(MinimalAPI, Blueprintable, meta=(CannotImplementInterfaceInBlueprint))
class UWeaponCarrierInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponCarrierWithOwnerSignature, AActor*, CarrierActor);

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

	UFUNCTION(BlueprintCallable, Category="WeaponCarrierInterface")
	virtual FWeaponActionDisplayInfo GetNextActionDisplayInfoByInput(EWeaponActionInput Input) const = 0;

	/** Equip given weapon. */
	virtual void EquipWeapon(UWeaponBase* NewWeapon) {}

	/** UnEquip current weapon. */
	virtual void UnEquipWeapon() {}

	/** Enqueue new input to equipping weapon. */
	virtual void EnqueueInput(EWeaponActionInput NewInput) {}

	/** Set the target actor that weapon attack to. */
	virtual void SetWeaponTargetActor(AActor* NewTarget) {}

	/** Set the target location that carrier move to. */
	virtual void SetWeaponTargetLocation(FVector NewLocation) {}

	/** Clear current weapon target actor. */
	virtual void ClearWeaponTargetActor() {}

	/** Move toward current target actor. */
	virtual void MoveToActionDistance() = 0;

	/** Set start distance for next action. */
	virtual void SetActionDistance(float NewDistance) = 0;

	/** Get current target actor. */
	virtual AActor* GetWeaponTargetActor() = 0;

	/** Get the delegate of weapon equipped event. */
	virtual FWeaponCarrierWithOwnerSignature& GetWeaponEquippedEvent() = 0;

	/** Get the delegate of weapon action finished. */
	virtual FWeaponCarrierWithOwnerSignature& GetWeaponActionFinishedEvent() = 0;

	/** Get the delegate of carrier reached the action distance. */
	virtual FWeaponCarrierWithOwnerSignature& GetCarrierReachActionDistanceEvent() = 0;
};
