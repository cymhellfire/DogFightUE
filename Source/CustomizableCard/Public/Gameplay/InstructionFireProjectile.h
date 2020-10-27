// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "HandleTargetInstructionBase.h"
#include "InstructionFireProjectile.generated.h"

class IGameProjectileInterface;

/**
 * Instruction that is able to shoot projectiles.
 */
UCLASS()
class CUSTOMIZABLECARD_API UInstructionFireProjectile : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionFireProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void HandleActorTarget(AActor* Target) override;
	virtual void HandlePositionTarget(FVector Position) override;
	virtual void HandleDirectionTarget(FVector Direction) override;

	/** Handle the projectile dead event. */
	UFUNCTION()
	void OnProjectileDead(AActor* Projectile);

	/** Spawn a projectile and launch it toward specified direction. */
	void SpawnProjectileAndLaunch(FVector Position, FRotator Rotation, FVector FireDirection, bool bMayHitSelf);

	APawn* GetOwnerControlledPawn() const;

	/** The class of projectile to spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction", meta=(MustImplement="GameProjectileInterface"))
	TSubclassOf<AActor> ProjectileClass;

	/** The initial speed of the projectile after launched. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	float MuzzleSpeed;

	/** How far the projectile will be spawned from user? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	float ProjectileSpawnDistance;

	/** The initial height of projectiles spawned. (In local space of character pawn.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	float ProjectileSpawnHeight;

	/** List of projectiles launched by this instruction. */
	TArray<IGameProjectileInterface*> ProjectileInstanceList;
};
