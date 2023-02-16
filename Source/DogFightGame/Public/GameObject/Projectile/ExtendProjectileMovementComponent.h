#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "ExtendProjectileMovementComponent.generated.h"

UENUM(BlueprintType)
namespace EProjectileHomingMode
{
	enum Type
	{
		Default				UMETA(ToolTip="Default mode that use acceleration toward target to modify velocity."),
		DirectionChange		UMETA(ToolTip="This mode will modify the direction of projectile velocity toward target."),
	};
}

UCLASS()
class DOGFIGHTGAME_API UExtendProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	UExtendProjectileMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual FVector ComputeVelocity(FVector InitialVelocity, float DeltaTime) const override;

	virtual FVector ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const override;

	void OnActivated();
	void OnDeactivated();

protected:
	void OnStartHomingTimerExpired();

	void OnStopHomingTimerExpired();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing")
	TEnumAsByte<EProjectileHomingMode::Type> HomingType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing", meta=(EditCondition="HomingType == EProjectileHomingMode::DirectionChange", EditConditionHides))
	float DirectionChangeRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing", meta=(ToolTip="Projectile with PeriodHoming enabled will only seek to target in specified period of time."))
	bool bPeriodHoming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing", meta=(EditCondition="bPeriodHoming==true", EditConditionHides))
	float StartHomingTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing", meta=(EditCondition="bPeriodHoming==true", EditConditionHides))
	float StopHomingTime;

private:
	TWeakObjectPtr<USceneComponent> PendingHomingTarget;

	FTimerHandle StartHomingTimerHandle;
	FTimerHandle StopHomingTimerHandle;
};
