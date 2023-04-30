#include "GameObject/Projectile/NewProjectileBase.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "GameInstance/DogFightGameInstance.h"
#include "GameObject/Projectile/ExtendProjectileMovementComponent.h"
#include "GameService/DamageService.h"
#include "GameService/GameEffectService.h"

ANewProjectileBase::ANewProjectileBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Turn on replication
	bReplicates = true;
	SetReplicatingMovement(true);

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
	DecayTime = 0.f;
}

void ANewProjectileBase::Reset()
{
	Super::Reset();

	// Clear the hit actor
	HitActor.Reset();

	// Remove the ignore launcher
	if (Launcher.IsValid())
	{
		CollisionComponent->IgnoreActorWhenMoving(Launcher.Get(), false);
	}
}

void ANewProjectileBase::LifeSpanExpired()
{
	// Invoke StopSimulating here and let actor pool handle this projectile
	MovementComponent->StopSimulating(FHitResult());
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
	if (WarheadData.GameEffectId >= 0)
	{
		// Spawn vfx
		UCommonGameplayFunctionLibrary::SpawnGameEffectAtPos(this, WarheadData.GameEffectId,
			GetActorLocation(), WarheadData.bUseProjectileRotation ? GetActorRotation() : FRotator::ZeroRotator);

		if (auto DamageService = UGameService::GetGameService<UDamageService>())
		{
			if (WarheadData.DamageRadius > 0.f)
			{
				
			}
			else
			{
				// Damage the hit target if no radius
				if (HitActor.IsValid())
				{
					UCommonGameplayFunctionLibrary::DamageActor(this, WarheadData.DamageId, HitActor.Get(), Damage, this);
				}
			}
		}
	}

	// Notify the client
	MulticastDead();

	// Trigger delegate
	OnProjectileDead.Broadcast(this);

	bAlive = false;

	// Check the decay time
	if (DecayTime > 0.f)
	{
		GetWorldTimerManager().SetTimer(DecayTimerHandle, this, &ANewProjectileBase::OnProjectileDecayTimerExpired, DecayTime);
	}
	else
	{
		OnProjectileReadyToRecycle.Broadcast(this);
	}
}

void ANewProjectileBase::MulticastDead_Implementation()
{
	// Disable collision
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Disable movement
	MovementComponent->StopMovementImmediately();
	MovementComponent->OnDeactivated();
}

void ANewProjectileBase::OnProjectileDecayTimerExpired()
{
	if (DecayTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DecayTimerHandle);
	}

	OnProjectileReadyToRecycle.Broadcast(this);
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
	// Record the hit target
	if (ImpactResult.bBlockingHit)
	{
		HitActor = ImpactResult.GetActor();
	}

	if (bDeadWhenStop)
	{
		Dead();
	}
}

void ANewProjectileBase::SetLauncher(AActor* InLauncher)
{
	Launcher = InLauncher;

	// Record the launcher as un-hittable
	CollisionComponent->IgnoreActorWhenMoving(InLauncher, true);
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

void ANewProjectileBase::HomingToTargetWithSpeed_Implementation(AActor* Target, float MuzzleSpeed, float Spread)
{
	if (!IsValid(Target))
	{
		return;
	}

	auto CurLocation = GetActorLocation();
	auto TargetDirection = (Target->GetActorLocation() - CurLocation).GetSafeNormal();

	// Calculate spread
	if (Spread > 0)
	{
		const FTransform FacingTransform(TargetDirection.Rotation());
		auto UpVector = FacingTransform.GetUnitAxis(EAxis::Z);
		TargetDirection = FMath::Lerp(TargetDirection, UpVector, Spread);
		// Rotate with random rotation
		TargetDirection = TargetDirection.RotateAngleAxis(FMath::RandRange(0.f, 360.f), FacingTransform.GetUnitAxis(EAxis::X));
	}

	auto MuzzleVelocity = TargetDirection * MuzzleSpeed;
	HomingToTargetWithVelocity(Target, MuzzleVelocity);
}
