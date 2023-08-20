// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject/Weapon/WeaponCommon.h"
#include "ArsenalComponent.generated.h"

class UWeaponBase;
class UWeaponDataAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UArsenalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UArsenalComponent();

	bool EquipWeapon(EWeaponSlotType Slot, const FPrimaryAssetId& InWeaponAsset);

	void AttackTarget(const FWeaponActionTarget& InTarget, EWeaponSlotType InSlot = EWeaponSlotType::WST_Primary);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OnWeaponDataLoaded(EWeaponSlotType Slot, UWeaponDataAsset* InWeaponData);

private:

	UPROPERTY(Transient)
	TMap<EWeaponSlotType, UWeaponBase*> WeaponSlotMap; 
};
