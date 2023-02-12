#pragma once

#include "GameService/LuaGameService.h"
#include "Interface/ActorPoolInterface/ActorPoolInterface.h"
#include "ProjectileService.generated.h"

class ANewProjectileBase;

/**
 * Data struct that configure a projectile type.
 */
USTRUCT(BlueprintType)
struct FProjectileConfigData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileConfigData")
	int32 ProjectileId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileConfigData")
	FString ResourcePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileConfigData")
	FString ScriptPath;
};

USTRUCT(BlueprintType)
struct FProjectileUtilsDesc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProjectileUtilsDesc")
	int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProjectileUtilsDesc")
	FString DisplayName;
};

/**
 * Minimal projectile config data for runtime usage.
 */
struct FRuntimeProjectileConfig
{
	int32 Id;
	UClass* Class;
	FString ScriptPath;
};

UCLASS()
class DOGFIGHTGAME_API UProjectileService : public ULuaGameService, public IActorPoolInterface
{
	GENERATED_BODY()

public:
	virtual void Startup() override;

	/**
	 * Spawn specified projectile by id.
	 * @param ProjectileId		Id of projectile to spawn.
	 * @param Pos				Position to place new projectile.
	 * @param Rot				Initial rotation of new projectile.
	 * @return					New spawned projectile.
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectileService")
	ANewProjectileBase* SpawnProjectileAtPos(int32 ProjectileId, FVector Pos, FRotator Rot);

	/**
	 * Get utility description of all projectiles.
	 * @return					List of utility description.
	 */
	UFUNCTION(BlueprintCallable, Category="ProjectileService")
	TArray<FProjectileUtilsDesc> GetAllProjectileUtilsDesc() const;

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.ProjectileService.ProjectileService";
	}

protected:
	void LoadConfig();

	ANewProjectileBase* SpawnNewProjectileInstance(const FRuntimeProjectileConfig* InConfig);

	/**
	 * Gather all projectile config data.
	 * @return			List of all projectile config data.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="ProjectileService")
	TArray<FProjectileConfigData> GatherProjectileConfigData();

	/**
	 * Get projectile name used when defined in config file.
	 * @param Id				Id of the projectile to get name.
	 * @return					Define name of corresponding projectile.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="ProjectileService")
	FString GetDefineNameOfProjectile(int32 Id) const;

	UFUNCTION()
	void OnProjectileDead(ANewProjectileBase* Projectile);

protected:

	TMap<int32, TSharedPtr<FRuntimeProjectileConfig>> ProjectileConfigMap;
};
