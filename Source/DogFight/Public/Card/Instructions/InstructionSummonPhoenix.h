// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "HandleTargetInstructionBase.h"
#include "InstructionSummonPhoenix.generated.h"

class AMoveToTargetVfx;
class IGameProjectileInterface;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionSummonPhoenix : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionSummonPhoenix(const FObjectInitializer& ObjectInitializer);

	virtual void ProcessTarget() override;

protected:

	void OnPhoenixReachDestination();

	UFUNCTION()
	void OnProjectileDead(AActor* Projectile);

public:
	/** The total damage phoenix can cause. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty TotalDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TSubclassOf<AActor> ProjectileClass;

	/** Distance to avoid projectile hit each other */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float ProjectileSpawnDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TSubclassOf<AMoveToTargetVfx> PhoenixVfxClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float PhoenixTargetZOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float PhoenixFireSpawnDelay;

protected:

	TArray<AActor*> TargetActorList;
	TArray<AActor*> ProjectileInstances;

	FVector PhoenixTargetPosition;

	FTimerHandle PhoenixTimerHandle;
};
