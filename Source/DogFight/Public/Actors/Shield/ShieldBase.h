// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Actors/Interfaces/GameplayTagsActorInterface.h"
#include "Actors/Vfx/VfxBase.h"
#include "Actors/Interfaces/GameShieldInterface.h"
#include "ShieldBase.generated.h"

class AProjectileBase;

/**
 * Shield type VFX that can block projectiles.
 */
UCLASS()
class DOGFIGHT_API AShieldBase : public AActor, public IGameShieldInterface, public IGameplayTagsActorInterface
{
	GENERATED_BODY()

public:
	AShieldBase();

	/** Check if this shield should block specified projectile. */
	virtual bool CheckShouldBlockProjectile(class AProjectileBase* TestProjectile);

#pragma region GameShieldInterface
	virtual void SetSourcePlayerController(AController* NewController) override;
	virtual void SetShieldBlockType(EShieldBlockType NewBlockType) override;
	virtual void SetAttachActor(AActor* NewParent) override;
	virtual void SetLifetime(int32 Lifetime) override;
#pragma endregion

#pragma region GameplayTagsActorInterface
	virtual void GetGameplayTags(FGameplayTagContainer& OutGameplayTags) override;
#pragma endregion

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void OnPlayerRoundEnd(int32 PlayerId);

	UFUNCTION()
	virtual void OnPlayerDead(int32 PlayerId);

	UFUNCTION()
	virtual void OnTargetActorDead();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shield")
	EShieldBlockType BlockType;

	/** The target socket when this shield attach to actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shield")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shield")
	UStaticMeshComponent* ShieldMesh;

	/** Source player controller. */
	AController* OwnerController;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shield")
	FGameplayTagContainer GameplayTags;

	/** A list of player rounds as the buff lifetime. */
	TArray<int32> LifetimeQueue;

	AActor* TargetActor;
};
