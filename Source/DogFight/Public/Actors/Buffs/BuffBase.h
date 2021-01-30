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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffProcessEndedSignature, ABuffBase*, Buff);
	FOnBuffProcessEndedSignature OnBuffProcessEndedEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndBuff();

	virtual void OnTargetPlayerRoundBegin();

	virtual void OnTargetPlayerRoundEnd();

	UFUNCTION()
	virtual void OnBuffProcessEnd();

	UFUNCTION()
	virtual void OnBuffEnded();

	virtual void ApplyBuff();

	virtual void RemoveBuff();

	virtual bool CheckBuffCompatibility(AActor* TestActor);

	virtual FText GetBuffStartText() const;

	virtual FText GetBuffEndText() const;

	void RegisterCallbackToCharacter(AActor* InActor);

	void UnregisterCallbackFromCharacter();

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

	/** The time cost of this buff when processed during round begin. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff", meta=(ClampMin="0.01"))
	float BuffDurationOnRoundBegin;

	/** The time cost of this buff when processed during round end. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff", meta=(ClampMin="0.01"))
	float BuffDurationOnRoundEnd;

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

	uint8 bEnded : 1;

	/** Whether this buff has infinite life time. */
	uint8 bPermanentBuff : 1;

	FTimerHandle BuffEndTimerHandle;

	FTimerHandle BuffProcessTimerHandle;
};
