﻿#pragma once

#include "CoreMinimal.h"
#include "GameProjectileInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameProjectileInterface : public  UInterface
{
	GENERATED_BODY()
};

/** Delegate which is fired when this projectile dead. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileDeadSignature, AActor*, Projectile);

/**
 * Interface that allows projectiles interact with card instruction.
 */
class IGameProjectileInterface
{
	GENERATED_BODY()

public:

	virtual void AdjustGravityScale(float NewGravityScale) {};

	virtual void SetDamage(float NewDamage) {};

	virtual void SetStrengthCost(float NewStrengthCost) {};

	virtual void SetDamageRadius(float NewDamageRadius) {};

	virtual void SetInitialSpeed(float NewSpeed) {};

	virtual void SetMaxSpeed(float NewSpeed) {};

	virtual void SetIgnoreCollisionAtStart(bool bIgnore) {};

	virtual void SetIgnoreActor(AActor* NewActor) {};

	virtual void SetIgnoreActors(TArray<AActor*> ActorList) {};

	virtual void LaunchAtDirection(const FVector& Direction) {};

	virtual void SetOwnerController(AController* NewController) {};

	virtual void SetOwnerCharacter(AActor* NewActor) {};

	virtual FProjectileDeadSignature& GetProjectileDeadDelegate() = 0;
};
