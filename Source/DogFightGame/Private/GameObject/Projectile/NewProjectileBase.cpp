#include "GameObject/Projectile/NewProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ANewProjectileBase::ANewProjectileBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Turn on replication
	bReplicates = true;

	// Create collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("Root");
	CollisionComponent->SetSphereRadius(50.f);
	SetRootComponent(CollisionComponent);

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	// Create projectile movement component
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->bRotationFollowsVelocity = true;

	Lifespan = 0.f;
}

void ANewProjectileBase::Reset()
{
	Super::Reset();

}

void ANewProjectileBase::LifeSpanExpired()
{
	// Invoke dead here and let actor pool handle this projectile
	Dead();
}

void ANewProjectileBase::OnActivated()
{
	// Register lifespan timer once activated
	if (Lifespan > 0)
	{
		SetLifeSpan(Lifespan);
	}

	// Reassign updated component to ensure movement component can update correctly
	MovementComponent->SetUpdatedComponent(CollisionComponent);
}

void ANewProjectileBase::Dead()
{
	// Clear the lifespan timer if it's still working
	if (TimerHandle_LifeSpanExpired.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_LifeSpanExpired);
	}

	// Trigger delegate
	OnProjectileDead.Broadcast(this);
}

void ANewProjectileBase::LaunchWithVelocity(const FVector& MuzzleVelocity)
{
	// Invoke activated
	OnActivated();

	// Set muzzle velocity
	MovementComponent->SetVelocityInLocalSpace(MuzzleVelocity);
}

void ANewProjectileBase::LaunchToTargetWithVelocity_Implementation(const FVector& Target, const FVector& MuzzleVelocity)
{
	LaunchWithVelocity(MuzzleVelocity);
}

void ANewProjectileBase::LaunchToTargetWithSpeed_Implementation(const FVector& Target, float MuzzleSpeed)
{
	auto CurLocation = GetActorLocation();
	auto TargetDirection = Target - CurLocation;
	auto MuzzleVelocity = TargetDirection.GetSafeNormal() * MuzzleSpeed;
	LaunchWithVelocity(MuzzleVelocity);
}
