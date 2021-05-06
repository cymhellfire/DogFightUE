// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorTeleportComponent.generated.h"

class AVfxBase;

UENUM(BlueprintType)
enum class EActorTeleportPhase : uint8
{
	ATP_Idle			UMETA(DisplayName="Idle"),
	ATP_Countdown		UMETA(DisplayName="Countdown"),
	ATP_Cooldown		UMETA(DisplayName="Cooldown"),
};

class UActorTeleportComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FTeleportComponentCountdownExpiredSignature, UActorTeleportComponent, OnTeleportStarted, UActorTeleportComponent*, TeleportComponent);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FTeleportComponentTeleportedSignature, UActorTeleportComponent, OnArrivedDestination, UActorTeleportComponent*, TeleportComponent);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FTeleportComponentCooldownExpiredSignature, UActorTeleportComponent, OnTeleportFinished, UActorTeleportComponent*, TeleportComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOGFIGHT_API UActorTeleportComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UActorTeleportComponent();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartTeleport(const FVector& Destination);

	UFUNCTION(BlueprintCallable, Category="TeleportComponent")
	EActorTeleportPhase GetCurrentPhase() const { return CurrentTeleportPhase; }

protected:
	void StartTeleport(FVector Destination);

	UFUNCTION()
	void OnTeleportCountdownExpired();

	UFUNCTION()
	void OnTeleportCooldownExpired();

public:
	/** Triggered when teleport is started (countdown started). */
	UPROPERTY(BlueprintAssignable, Category="TeleportComponent")
	FTeleportComponentCountdownExpiredSignature OnTeleportStarted;

	/** Triggered when actor is teleported to destination. */
	UPROPERTY(BlueprintAssignable, Category="TeleportComponent")
	FTeleportComponentTeleportedSignature OnArrivedDestination;

	/** Triggered when actual teleport finished (cooldown finished). */
	UPROPERTY(BlueprintAssignable, Category="TeleportComponent")
	FTeleportComponentCooldownExpiredSignature OnTeleportFinished;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TeleportComponent")
	float ZAxisCorrectionOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TeleportComponent")
	float TeleportCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TeleportComponent")
	float TeleportCountdown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TeleportComponent")
	TSubclassOf<AVfxBase> TeleportOriginVfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TeleportComponent")
	TSubclassOf<AVfxBase> TeleportDestinationVfx;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TeleportComponent")
	EActorTeleportPhase CurrentTeleportPhase;

	FVector CacheDestination;
	FTimerHandle TeleportTimerHandle;
};
