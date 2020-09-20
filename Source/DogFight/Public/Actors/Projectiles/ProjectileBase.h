// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameProjectileInterface.h"

#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DOGFIGHT_API AProjectileBase : public AActor, public IGameProjectileInterface
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileDeadSignature, AProjectileBase*, Projectile);

	UPROPERTY(BlueprintAssignable, Category="Projectile")
	FProjectileDeadSignature OnProjectileDead;

	// Sets default values for this actor's properties
	AProjectileBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Execute when projectile dead.
	 */
	virtual void Dead();

#pragma region GameProjectileInterface
	virtual void AdjustGravityScale(float NewGravityScale) override;
	virtual void SetInitialSpeed(float NewSpeed) override;
	virtual void SetMaxSpeed(float NewSpeed) override;
	virtual void LaunchAtDirection(const FVector& Direction) override;
#pragma endregion GameProjectileInterface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	/** Handle the collision of projectiles */
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	bool DeadOnHit;

protected:

	UPROPERTY(VisibleDefaultsOnly, Category="Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category="Projectile")
	UProjectileMovementComponent* MovementComponent;
};
