// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameBuffInterface.h"
#include "BuffBase.generated.h"

UCLASS(Abstract)
class DOGFIGHT_API ABuffBase : public AActor, public IGameBuffInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABuffBase();

	virtual void ApplyBuff() {};

	virtual void RemoveBuff() {};

#pragma region Interface
	virtual void SetLifetime(float NewLifetime) override;

	virtual void SetSourcePlayerController(APlayerController* PlayerController) override;
#pragma endregion Interface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BuffEnd();

	virtual void OnPlayerRoundEnd(int32 PlayerId);

	virtual void OnPlayerDead(int32 PlayerId);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	int32 Lifetime;

protected:

	/** Id of source player. */
	APlayerController* SourcePlayerController;

	/** A list of player rounds as the buff lifetime. */
	TArray<int32> LifetimeQueue;
};
