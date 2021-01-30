// Dog Fight Game Code By CYM.


#include "Actors/Projectiles/TriggerableProjectile.h"

ATriggerableProjectile::ATriggerableProjectile()
	: Super()
{
	bTriggered = false;
}

void ATriggerableProjectile::SetTriggered(bool NewTriggered)
{
	bTriggered = NewTriggered;

	if (bTriggered && MovementComponent != nullptr)
	{
		MovementComponent->AddForce(ForceAfterTriggered);
	}
}

void ATriggerableProjectile::ReactivateProjectile()
{
	if (MovementComponent != nullptr)
	{
		// Re-activate the projectile movement
		if (MovementComponent->HasStoppedSimulation())
		{
			MovementComponent->SetUpdatedComponent(CollisionComponent);
		}
	}

	// if (CollisionComponent != nullptr)
	// {
	// 	CollisionComponent->WakeAllRigidBodies();
	// }
}

bool ATriggerableProjectile::CheckDeadOnHitCondition(AActor* OtherActor)
{
	if (!bTriggered)
	{
		return false;
	}

	return Super::CheckDeadOnHitCondition(OtherActor);
}
