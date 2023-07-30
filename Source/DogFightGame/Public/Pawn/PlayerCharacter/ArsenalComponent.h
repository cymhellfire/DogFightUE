// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject/Weapon/WeaponCommon.h"
#include "ArsenalComponent.generated.h"

class UWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UArsenalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UArsenalComponent();

	bool EquipWeapon(EWeaponSlotType Slot, UWeaponBase* InWeapon);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(Transient)
	TMap<EWeaponSlotType, UWeaponBase*> WeaponSlotMap; 
};
