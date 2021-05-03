// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "GameplayTagContainer.h"

#include "Pawns/StandardModePlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Actors/Interfaces/GameProjectileInterface.h"
#include "Actors/Interfaces/GameplayTagsActorInterface.h"

#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AVfxBase;
class AShieldBase;
class UAudioComponent;

UCLASS()
class DOGFIGHT_API AProjectileBase : public AActor, public IGameProjectileInterface, public IGameplayTagsActorInterface
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
	UFUNCTION(BlueprintCallable, Category="DogFight|Projectile")
	virtual void Dead();

	virtual void BeginDestroy() override;

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

#pragma region GameplayTagsActorInterface
	virtual void GetGameplayTags(FGameplayTagContainer& OutGameplayTags) override;
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupShield();

	virtual bool CheckDeadOnHitCondition(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastIgnoreActorWhileMoving(AActor* Target, bool bShouldIgnore);

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
	void OnLifeTimerExpired();

	UFUNCTION()
	void OnDecayTimerFinished();

	UFUNCTION()
	void OnShieldRegistered(AShieldBase* NewShield);

	UFUNCTION()
	void OnShieldUnregistered(AShieldBase* Shield);

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

	/** Only dead when hit target match this query. (None means any actor is match.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile", meta=(EditCondition="DeadOnHit"))
	FGameplayTagQuery HitDeadTagQuery;

	/** How long this projectile stay before self-destruction. (0 means permanent.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile", meta=(ClampMin="0"))
	float Lifetime;

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

	uint8 bIsAlive : 1;

	FTimerHandle LifeTimerHandle;

	FTimerHandle DecayTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	FGameplayTagContainer GameplayTags;
};
