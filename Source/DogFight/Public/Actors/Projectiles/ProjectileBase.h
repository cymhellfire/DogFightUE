// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"

#include "StandardModePlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameProjectileInterface.h"

#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AVfxBase;
class UAudioComponent;

UCLASS()
class DOGFIGHT_API AProjectileBase : public AActor, public IGameProjectileInterface
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category="Projectile")
	FProjectileDeadSignature OnProjectileDead;

	// Sets default values for this actor's properties
	AProjectileBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	/**
	 * Execute when projectile dead.
	 */
	virtual void Dead();

#pragma region GameProjectileInterface
	virtual void AdjustGravityScale(float NewGravityScale) override;
	virtual void SetDamage(float NewDamage) override;
	virtual void SetDamageRadius(float NewDamageRadius) override;
	virtual void SetInitialSpeed(float NewSpeed) override;
	virtual void SetMaxSpeed(float NewSpeed) override;
	virtual void SetIgnoreCollisionAtStart(bool bIgnore) override;
	virtual void LaunchAtDirection(const FVector& Direction) override;
	virtual void SetOwnerController(AController* NewController) override;
	virtual void SetOwnerCharacter(AActor* NewActor) override;
	virtual FProjectileDeadSignature& GetProjectileDeadDelegate() override { return OnProjectileDead; }
#pragma endregion GameProjectileInterface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	/** Handle the collision of projectiles */
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnRep_MaxSpeed();

	UFUNCTION()
	void OnRep_InitialSpeed();

	UFUNCTION()
	void OnRep_GravityScale();

	UFUNCTION()
	void OnRep_LaunchVelocity();

	UFUNCTION()
	void OnDecayTimerFinished();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<UDamageType> DamageType;

	/** The radius of damage area. (0 means only damage the hit target.)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	bool DeadOnHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<AVfxBase> VfxOnDead;

	/** Whether make the Vfx forward direction align with the hit normal vector. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	bool AlignVfxWithHitNormal;

	/** How long does this projectile stay after dead? (Wait for Vfx or etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float DecayDuration;

	/** Whether this projectile ignore its owner at the start. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	bool bIgnoreOwnerCollisionAtStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	AController* OwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	AStandardModePlayerCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile", ReplicatedUsing=OnRep_MaxSpeed)
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile", ReplicatedUsing=OnRep_InitialSpeed)
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile", ReplicatedUsing=OnRep_GravityScale)
	float GravityScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile", ReplicatedUsing=OnRep_LaunchVelocity)
	FVector LaunchVelocity;

protected:

	UPROPERTY(VisibleDefaultsOnly, Category="Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category="Projectile")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category="Projectile")
	UAudioComponent* AudioComponent;

	AActor* HitActor;

	FTimerHandle DecayTimerHandle;
};
