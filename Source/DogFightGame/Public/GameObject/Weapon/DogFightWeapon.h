// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Object/WeaponBase.h"
#include "DogFightWeapon.generated.h"

/**
 * Weapon class used in Dog Fight project.
 */
UCLASS()
class DOGFIGHTGAME_API UDogFightWeapon : public UWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void OnHitTarget(AActor* TargetActor, UPrimitiveComponent* TargetComponent, const FHitResult& HitResult) override;
};
