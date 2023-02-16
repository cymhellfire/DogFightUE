#include "GameObject/Projectile/NewProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameInstance/DogFightGameInstance.h"
#include "GameObject/Projectile/ExtendProjectileMovementComponent.h"
#include "GameObject/Warhead/WarheadBase.h"
#include "GameService/GameEffectService.h"

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
	MovementComponent = CreateDefaultSubobject<UExtendProjectileMovementComponent>("MovementComponent");
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->OnProjectileStop.AddDynamic(this, &ANewProjectileBase::OnProjectileStopped);

	// Initial values
	Lifetime = 0.f;
	bDeadWhenStop = true;
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
	if (Lifetime > 0)
	{
		SetLifeSpan(Lifetime);
	}

	// Reassign updated component to ensure movement component can update correctly
	MovementComponent->SetUpdatedComponent(CollisionComponent);
	MovementComponent->OnActivated();

	// Enable collision
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Mark projectile as alive
	bAlive = true;
}

void ANewProjectileBase::Dead()
{
	if (!bAlive)
		return;

	// Clear the lifespan timer if it's still working
	if (TimerHandle_LifeSpanExpired.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_LifeSpanExpired);
	}

	// Use warhead
	if (!Warhead.IsNull())
	{
		auto WarheadPtr = Warhead.IsValid() ? Warhead.Get() : Warhead.LoadSynchronous();
		if (WarheadPtr)
		{
			if (auto GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
			{
				if (auto GameEffectService = Cast<UGameEffectService>(GameInstance->GetGameServiceBySuperClass(UGameEffectService::StaticClass())))
				{
					GameEffectService->SpawnEffectAtPos(WarheadPtr->GameEffectId, GetActorLocation(),
						WarheadPtr->bUseProjectileRotation ? GetActorRotation() : FRotator::ZeroRotator);
				}
			}
		}
	}

	// Disable collision
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Trigger delegate
	OnProjectileDead.Broadcast(this);

	bAlive = false;
}

void ANewProjectileBase::LaunchWithVelocity(const FVector& MuzzleVelocity)
{
	// Invoke activated
	OnActivated();

	// Set muzzle velocity
	MovementComponent->SetVelocityInLocalSpace(MuzzleVelocity);
}

void ANewProjectileBase::OnProjectileStopped(const FHitResult& ImpactResult)
{
	if (bDeadWhenStop)
	{
		Dead();
	}
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

void ANewProjectileBase::HomingToTargetWithVelocity_Implementation(AActor* Target, const FVector& MuzzleVelocity)
{
	if (!IsValid(Target))
	{
		return;
	}

	// Set the homing target
	MovementComponent->HomingTargetComponent = Target->GetRootComponent();
	LaunchWithVelocity(MuzzleVelocity);
}

void ANewProjectileBase::HomingToTargetWithSpeed_Implementation(AActor* Target, float MuzzleSpeed)
{
	if (!IsValid(Target))
	{
		return;
	}

	auto CurLocation = GetActorLocation();
	auto TargetDirection = Target->GetActorLocation() - CurLocation;
	auto MuzzleVelocity = TargetDirection.GetSafeNormal() * MuzzleSpeed;
	HomingToTargetWithVelocity(Target, MuzzleVelocity);
}
