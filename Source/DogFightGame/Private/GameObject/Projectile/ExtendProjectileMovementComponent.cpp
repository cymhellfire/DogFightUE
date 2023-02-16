#include "GameObject/Projectile/ExtendProjectileMovementComponent.h"

UExtendProjectileMovementComponent::UExtendProjectileMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DirectionChangeRatio = 0.2f;
	bPeriodHoming = false;
	StartHomingTime = 0;
	StopHomingTime = 1.f;
}

FVector UExtendProjectileMovementComponent::ComputeVelocity(FVector InitialVelocity, float DeltaTime) const
{
	// Calculate the acceleration this frame
	const FVector Acceleration = ComputeAcceleration(InitialVelocity, DeltaTime);

	// Modify the current velocity before apply acceleration
	if (HomingTargetComponent.IsValid())
	{
		// DirectionChange mode should rotate the current velocity toward target
		if (HomingType == EProjectileHomingMode::DirectionChange)
		{
			auto TargetDir = (HomingTargetComponent->GetComponentLocation() - UpdatedComponent->GetComponentLocation()).GetSafeNormal();
			auto CurrentDir = InitialVelocity.GetSafeNormal();

			auto NewDir = FMath::Lerp(CurrentDir, TargetDir, DirectionChangeRatio * DeltaTime);
			InitialVelocity = NewDir * InitialVelocity.Length();
		}
	}

	// v = v0 + a*t
	FVector NewVelocity = InitialVelocity + (Acceleration * DeltaTime);

	return LimitVelocity(NewVelocity);
}

FVector UExtendProjectileMovementComponent::ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const
{
	if (HomingType == EProjectileHomingMode::DirectionChange)
	{
		// DirectionChange mode will modify the final velocity instead of the acceleration
		return FVector::ZeroVector;
	}

	return Super::ComputeHomingAcceleration(InVelocity, DeltaTime);
}

void UExtendProjectileMovementComponent::OnActivated()
{
	if (bPeriodHoming)
	{
		// Setup period homing countdown
		if (StartHomingTime > 0)
		{
			// Record the current homing target for later recover
			PendingHomingTarget = HomingTargetComponent;
			HomingTargetComponent.Reset();
			GetWorld()->GetTimerManager().SetTimer(StartHomingTimerHandle, this, &UExtendProjectileMovementComponent::OnStartHomingTimerExpired, StartHomingTime);
		}
		if (StopHomingTime > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(StopHomingTimerHandle, this, &UExtendProjectileMovementComponent::OnStopHomingTimerExpired, StopHomingTime);
		}
	}
}

void UExtendProjectileMovementComponent::OnDeactivated()
{
	if (bPeriodHoming)
	{
		if (StartHomingTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(StartHomingTimerHandle);
		}
		if (StopHomingTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(StopHomingTimerHandle);
		}
	}
}

void UExtendProjectileMovementComponent::OnStartHomingTimerExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(StartHomingTimerHandle);

	// Recover homing target
	if (PendingHomingTarget.IsValid())
	{
		HomingTargetComponent = PendingHomingTarget;
	}
}

void UExtendProjectileMovementComponent::OnStopHomingTimerExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(StopHomingTimerHandle);

	// Clear homing target
	if (HomingTargetComponent.IsValid())
	{
		HomingTargetComponent.Reset();
	}
}
