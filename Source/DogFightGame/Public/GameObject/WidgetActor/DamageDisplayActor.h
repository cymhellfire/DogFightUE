#pragma once

#include "CoreMinimal.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "DamageDisplayActor.generated.h"

class ADamageDisplayActor;
class UDamageDisplayWidget;
class UWidgetLocatorComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDisplayActorDead, ADamageDisplayActor*, Instance);

UCLASS()
class DOGFIGHTGAME_API ADamageDisplayActor : public AActor
{
	GENERATED_BODY()

public:
	ADamageDisplayActor();

	UFUNCTION(BlueprintCallable, Category="DamageDisplayActor")
	void SetWidget(UDamageDisplayWidget* InWidget);

	UFUNCTION(BlueprintCallable, Category="DamageDisplayActor")
	void SetDamageInfo(const FDamageDisplayParams& DisplayParams);

	void Activate(float Lifetime);

	virtual void LifeSpanExpired() override;

protected:
	void Dead();

	UFUNCTION()
	void NoCollisionTimerExpired();

public:
	float CollisionDuration;

	FOnDamageDisplayActorDead OnDamageDisplayActorDead;
	FOnDamageDisplayActorDead OnDamageDisplayActorNoCollision;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DamageDisplayActor")
	UWidgetLocatorComponent* LocatorComponent;

	UPROPERTY(Transient)
	UDamageDisplayWidget* DamageDisplayWidget;

	FTimerHandle NoCollisionHandle;
};
