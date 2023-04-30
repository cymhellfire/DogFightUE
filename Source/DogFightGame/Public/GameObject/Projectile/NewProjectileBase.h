#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "GameObject/Warhead/Warhead.h"
#include "NewProjectileBase.generated.h"

class USphereComponent;
class UExtendProjectileMovementComponent;

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
	 * Set the actor who instigate this launch.
	 * @param InLauncher	Instigator.
	 */
	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetLauncher(AActor* InLauncher);

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

	/**
	 * Launch this projectile with muzzle velocity and let this projectile seek to target actor.
	 * Projectile with bIsHomingProjectile off won't act as expect.
	 * @param Target			Target actor that this projectile homing.
	 * @param MuzzleVelocity	Initial velocity of this projectile.
	 */
	UFUNCTION(BlueprintCallable, Category="Projectile", NetMulticast, Reliable)
	void HomingToTargetWithVelocity(AActor* Target, const FVector& MuzzleVelocity);

	/**
	 * Launch this projectile with muzzle velocity and let this projectile seek to target actor.
	 * Projectile with bIsHomingProjectile off won't act as expect.
	 * @param Target			Target actor that this projectile homing.
	 * @param MuzzleSpeed		The initial length of muzzle velocity.
	 * @param Spread			How spread the projectile will be.
	 */
	UFUNCTION(BlueprintCallable, Category="Projectile", NetMulticast, Reliable)
	void HomingToTargetWithSpeed(AActor* Target, float MuzzleSpeed, float Spread);
protected:
	void OnActivated();
	void Dead();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDead();

	void LaunchWithVelocity(const FVector& MuzzleVelocity);

	UFUNCTION()
	void OnProjectileStopped(const FHitResult& ImpactResult);

	UFUNCTION()
	void OnProjectileDecayTimerExpired();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	int32 Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float Lifetime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	int32 WarheadId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	FWarhead WarheadData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	bool bDeadWhenStop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float DecayTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float Damage;

	UPROPERTY(BlueprintAssignable, Category="Projectile")
	FProjectileDeadDelegate OnProjectileDead;

	UPROPERTY(BlueprintAssignable, Category="Projectile")
	FProjectileDeadDelegate OnProjectileReadyToRecycle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	UExtendProjectileMovementComponent* MovementComponent;

	uint8 bAlive : 1;

	FTimerHandle DecayTimerHandle;

	/** The actor this projectile hit. */
	TWeakObjectPtr<AActor> HitActor;

	/** The launcher actor this time. */
	TWeakObjectPtr<AActor> Launcher;
};
