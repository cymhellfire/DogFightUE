#pragma once

#include "GameService/LuaGameService.h"
#include "Interface/ActorPoolInterface/ActorPoolInterface.h"
#include "ProjectileService.generated.h"

class ANewProjectileBase;

UCLASS()
class DOGFIGHTGAME_API UProjectileService : public ULuaGameService, public IActorPoolInterface
{
	GENERATED_BODY()

public:
	UProjectileService();

	/**
	 * Spawn specified projectile by id.
	 * @param ProjectileId		Id of projectile to spawn.
	 * @param Pos				Position to place new projectile.
	 * @param Rot				Initial rotation of new projectile.
	 * @return					New spawned projectile.
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectileService")
	ANewProjectileBase* SpawnProjectileAtPos(int32 ProjectileId, FVector Pos, FRotator Rot);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.ProjectileService.ProjectileService";
	}

protected:
	ANewProjectileBase* SpawnNewProjectileInstance(int32 InId);

	UFUNCTION(BlueprintImplementableEvent, Category="ProjectileService")
	ANewProjectileBase* SpawnNewProjectileInstance_Implementation(int32 InId);

	UFUNCTION()
	void OnProjectileDead(ANewProjectileBase* Projectile);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ProjectileService")
	FVector ActorPoolPosition;
};
