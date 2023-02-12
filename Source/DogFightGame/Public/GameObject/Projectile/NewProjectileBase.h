#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "NewProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UWarheadBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileDeadDelegate, class ANewProjectileBase*, Projectile);

UCLASS()
class DOGFIGHTGAME_API ANewProjectileBase : public AActor, public IUnLuaInterface
{
	GENERATED_BODY()

public:
	ANewProjectileBase(const FObjectInitializer& ObjectInitializer);

	virtual void Reset() override;
	/** Integrate actor pool logic here. */
	virtual void LifeSpanExpired() override;

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Projectiles.ProjectileBase";
	}

	/**
	 * Load the logic script to this projectile.
	 * @param Path			Path of the script.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Projectile")
	void SetProjectileScriptModule(const FString& Path);

	/**
	 * Launch this projectile toward target location with given muzzle velocity.
	 * The muzzle velocity might be not point to target.
	 * @param Target			Target location.
	 * @param MuzzleVelocity	Initial velocity of this projectile.
	 */
	UFUNCTION(BlueprintCallable, Category="Projectile", NetMulticast, Reliable)
	void LaunchToTargetWithVelocity(const FVector& Target, const FVector& MuzzleVelocity);

	/**
	 * Launch this projectile toward target location with given initial speed.
	 * The muzzle velocity will keep pointing to target.
	 * @param Target			Target location.
	 * @param MuzzleSpeed		The initial length of muzzle velocity.
	 */
	UFUNCTION(BlueprintCallable, Category="Projectile", NetMulticast, Reliable)
	void LaunchToTargetWithSpeed(const FVector& Target, float MuzzleSpeed);

protected:
	void OnActivated();
	void Dead();

	void LaunchWithVelocity(const FVector& MuzzleVelocity);

	UFUNCTION()
	void OnProjectileStopped(const FHitResult& ImpactResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	int32 Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float Lifetime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSoftObjectPtr<UWarheadBase> Warhead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	bool bDeadWhenStop;

	UPROPERTY(BlueprintAssignable, Category="Projectile")
	FProjectileDeadDelegate OnProjectileDead;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	UProjectileMovementComponent* MovementComponent;

	uint8 bAlive : 1;
};
