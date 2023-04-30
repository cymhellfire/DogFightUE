#include "GameService/ProjectileService.h"

#include "Common/DogFightGameLog.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameObject/Projectile/NewProjectileBase.h"

UProjectileService::UProjectileService()
{
	ActorPoolPosition = FVector(0, 0, -20000);
}

ANewProjectileBase* UProjectileService::SpawnProjectileAtPos(int32 ProjectileId, FVector Pos, FRotator Rot)
{
	// Get instance from actor pool first
	ANewProjectileBase* Result = GetInstanceById<ANewProjectileBase>(ProjectileId);

	// Create new instance if no one available
	if (Result == nullptr)
	{
		Result = SpawnNewProjectileInstance(ProjectileId);
	}

	// Ensure the projectile placed at the desired position
	if (Result)
	{
		Result->SetActorLocation(Pos);
		Result->SetActorRotation(Rot);
	}

	return Result;
}

ANewProjectileBase* UProjectileService::SpawnNewProjectileInstance(int32 InId)
{
	if (auto NewInstance = SpawnNewProjectileInstance_Implementation(InId))
	{
		NewInstance->Id = InId;
		NewInstance->OnProjectileReadyToRecycle.AddDynamic(this, &UProjectileService::OnProjectileDead);
		return NewInstance;
	}

	return nullptr;
}

void UProjectileService::OnProjectileDead(ANewProjectileBase* Projectile)
{
	if (IsValid(Projectile))
	{
		// Reset projectile before reclaim it
		Projectile->Reset();
		ReclaimInstance(Projectile->Id, Projectile);
		Projectile->SetActorLocation(ActorPoolPosition);
	}
}
