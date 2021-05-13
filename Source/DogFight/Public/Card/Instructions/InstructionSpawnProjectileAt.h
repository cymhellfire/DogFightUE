// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Actors/Interfaces/GameProjectileInterface.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "InstructionSpawnProjectileAt.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionSpawnProjectileAt : public UHandleTargetInstructionBase
{
	GENERATED_UCLASS_BODY()


protected:
	virtual void HandlePositionTarget(FVector Position) override;
	virtual bool HandleActorTarget(AActor* Target) override;
	virtual void HandleDirectionTarget(FVector Direction) override;
	
	void SpawnProjectile(FVector Position, FRotator Rotation);

	UFUNCTION()
	void OnProjectileDead(AActor* Projectile);
public:

	/** The class of projectile to spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(MustImplement="GameProjectileInterface"))
	TSubclassOf<AActor> ProjectileClass;

	/** The offset of spawning position to target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FVector SpawnPositionOffset;

	/** The facing direction after projectile spawned. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FVector InitialFacingDirection;

	/** The initial speed after spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty InitialSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty StrengthCost;

protected:

	/** List of projectiles spawned by this instruction. */
	TArray<IGameProjectileInterface*> ProjectileInstances;
};
