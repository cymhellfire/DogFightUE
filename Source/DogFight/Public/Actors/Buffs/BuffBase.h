// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameBuffInterface.h"

#include "BuffBase.generated.h"

class AVfxBase;

UCLASS(Abstract)
class DOGFIGHT_API ABuffBase : public AActor, public IGameBuffInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABuffBase();

#pragma region Interface
	virtual void SetLifetime(float NewLifetime) override;

	virtual void SetSourcePlayerController(AController* PlayerController) override;

	virtual void SetTargetActor(AActor* Target) override;
#pragma endregion Interface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BuffEnd();

	virtual void ApplyBuff();

	virtual void RemoveBuff();

	UFUNCTION()
	virtual void OnPlayerRoundEnd(int32 PlayerId);

	UFUNCTION()
	virtual void OnPlayerDead(int32 PlayerId);

	UFUNCTION()
	virtual void OnTargetActorDead();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	int32 Lifetime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	TSubclassOf<AVfxBase> VfxClass;

protected:

	/** Id of source player. */
	AController* SourcePlayerController;

	/** A list of player rounds as the buff lifetime. */
	TArray<int32> LifetimeQueue;

	/** Target Actor of this buff. */
	AActor* TargetActor;

	/** The Vfx Actor spawned. */
	AVfxBase* VfxActor;

	/** Whether this buff is applied to target. */
	bool bAppliedToTarget;
};
