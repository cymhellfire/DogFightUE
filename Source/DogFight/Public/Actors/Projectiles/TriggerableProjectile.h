// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/ProjectileBase.h"
#include "TriggerableProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ATriggerableProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	ATriggerableProjectile();

	UFUNCTION(BlueprintCallable, Category="TriggerableProjectile")
	void SetTriggered(bool NewTriggered);

	/** Re-activate this projectile to let it movable again. */
	void ReactivateProjectile();

protected:

	virtual bool CheckDeadOnHitCondition(AActor* OtherActor) override;

protected:
	/** This force will be applied after this projectile is triggered. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TriggerableProjectile")
	FVector ForceAfterTriggered;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TrigggerableProjectile")
	uint8 bTriggered : 1;
};
