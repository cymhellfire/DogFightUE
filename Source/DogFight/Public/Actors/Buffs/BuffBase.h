// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameBuffInterface.h"
#include "Common/LocalizedString.h"

#include "BuffBase.generated.h"

class AVfxBase;

UCLASS(Abstract)
class DOGFIGHT_API ABuffBase : public AActor, public IGameBuffInterface
{
	GENERATED_BODY()
	
public:
	friend class UBuffQueue;

	// Sets default values for this actor's properties
	ABuffBase();

	/** Whether this buff should be ended in this round. */
	bool IsBuffEnding() const { return bPendingEnd; }

#pragma region Interface
	virtual void SetLifetime(float NewLifetime) override;

	virtual void SetSourcePlayerController(AController* PlayerController) override;

	virtual void SetTargetActor(AActor* Target) override;

	virtual bool IsCompatibleWith(AActor* Target) override;
#pragma endregion Interface

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffEndedSignature, ABuffBase*, Buff);
	FOnBuffEndedSignature OnBuffEndedEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndBuff();

	UFUNCTION()
	virtual void OnBuffEnded();

	virtual void ApplyBuff();

	virtual void RemoveBuff();

	virtual bool CheckBuffCompatibility(AActor* TestActor);

	virtual FText GetBuffStartText() const;

	virtual FText GetBuffEndText() const;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	float BuffEndingDuration;

	/** How many same buffs can be added to same target? (0 means no limitation.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff", meta=(ClampMin="0"))
	int32 MaxCountPerTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	FLocalizedString BuffStartText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	FLocalizedString BuffEndText;

	/** Id of source player. */
	AController* SourcePlayerController;

	/** A list of player rounds as the buff lifetime. */
	TArray<int32> LifetimeQueue;

	/** Target Actor of this buff. */
	AActor* TargetActor;

	/** The Vfx Actor spawned. */
	AVfxBase* VfxActor;

	/** Whether this buff is applied to target. */
	uint8 bAppliedToTarget : 1;

	/** Whether this buff should be ended in this round. */
	uint8 bPendingEnd : 1;

	FTimerHandle BuffEndTimerHandle;
};
