// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "HandleTargetInstructionBase.h"
#include "InstructionFireProjectile.generated.h"

class IGameProjectileInterface;

UENUM(BlueprintType)
enum class ESpawnLocationType : uint8
{
	SLT_Offset			UMETA(DisplayName="Offset", Tooltip="Offset based on user pawn location."),
	SLT_Socket			UMETA(DisplayName="Socket", Tooltip="At the specified socket location of user pawn."),
};

/**
 * Instruction that is able to shoot projectiles.
 */
UCLASS()
class DOGFIGHT_API UInstructionFireProjectile : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionFireProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool HandleActorTarget(AActor* Target) override;
	virtual void HandlePositionTarget(FVector Position) override;
	virtual void HandleDirectionTarget(FVector Direction) override;

	/** Handle the projectile dead event. */
	UFUNCTION()
	void OnProjectileDead(AActor* Projectile);

	/** Spawn a projectile and launch it toward specified direction. */
	void SpawnProjectileAndLaunch(FVector Position, FRotator Rotation, FVector FireDirection, bool bMayHitSelf);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty Damage;

	/** The class of projectile to spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction", meta=(MustImplement="GameProjectileInterface"))
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty DamageRadius;

	/** The initial speed of the projectile after launched. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	FUpgradableIntProperty MuzzleSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	ESpawnLocationType SpawnLocationType;

	/** How far the projectile will be spawned from user? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction", meta=(EditCondition="SpawnLocationType==ESpawnLocationType::SLT_Offset"))
	float ProjectileSpawnDistance;

	/** The initial height of projectiles spawned. (In local space of character pawn.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction", meta=(EditCondition="SpawnLocationType==ESpawnLocationType::SLT_Offset"))
	float ProjectileSpawnHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="SpawnLocationType==ESpawnLocationType::SLT_Socket"))
	FName SocketName;

protected:
	/** List of projectiles launched by this instruction. */
	TArray<IGameProjectileInterface*> ProjectileInstanceList;
};
