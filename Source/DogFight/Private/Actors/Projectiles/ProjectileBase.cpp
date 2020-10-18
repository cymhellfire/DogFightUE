// Dog Fight Game Code By CYM.


#include "ProjectileBase.h"

#include "CustomizableCard.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Replicate this actor for network spawn
	bReplicates = true;

	// Create collision
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = CollisionComponent;

	// Create Projectile Movement Component
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
	MovementComponent->bRotationFollowsVelocity = true;

	DeadOnHit = true;
	DecayDuration = 0;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit on %s"), *OtherActor->GetName()));

	// Damage hit actor on server
	if (OtherActor != nullptr && GetLocalRole() == ROLE_Authority)
	{
		OtherActor->TakeDamage(BaseDamage, FDamageEvent{DamageType}, OwnerController, OwnerCharacter);
	}

	if (DeadOnHit)
	{
		Dead();
	}
}

void AProjectileBase::OnRep_MaxSpeed()
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->MaxSpeed = MaxSpeed;
	}
}

void AProjectileBase::OnRep_InitialSpeed()
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->InitialSpeed = InitialSpeed;
	}
}

void AProjectileBase::OnRep_GravityScale()
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->ProjectileGravityScale = GravityScale;
	}
}

void AProjectileBase::OnRep_LaunchVelocity()
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->Velocity = LaunchVelocity;
	}
}

void AProjectileBase::OnDecayTimerFinished()
{
	// Clear timer handle
	GetWorldTimerManager().ClearTimer(DecayTimerHandle);

	// Destroy this projectile
	// TODO: Pool projectiles in the future
	Destroy();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileBase, MaxSpeed);
	DOREPLIFETIME(AProjectileBase, InitialSpeed);
	DOREPLIFETIME(AProjectileBase, GravityScale);
	DOREPLIFETIME(AProjectileBase, LaunchVelocity);
}

void AProjectileBase::Dead()
{
	// Broadcast the event
	OnProjectileDead.Broadcast(this);

	if (DecayDuration > 0.f)
	{
		GetWorldTimerManager().SetTimer(DecayTimerHandle, this, &AProjectileBase::OnDecayTimerFinished, DecayDuration);
	}
	else
	{
		// Destroy this projectile
		// TODO: Pool projectiles in the future
		Destroy();
	}
}

void AProjectileBase::AdjustGravityScale(float NewGravityScale)
{
	if (GetNetMode() != NM_Client)
	{
		GravityScale = NewGravityScale;

		OnRep_GravityScale();
	}
}

void AProjectileBase::SetInitialSpeed(float NewSpeed)
{
	if (GetNetMode() != NM_Client)
	{
		InitialSpeed = NewSpeed;

		OnRep_InitialSpeed();
	}
}

void AProjectileBase::SetMaxSpeed(float NewSpeed)
{
	if (GetNetMode() != NM_Client)
	{
		MaxSpeed = NewSpeed;

		OnRep_MaxSpeed();
	}
}

void AProjectileBase::LaunchAtDirection(const FVector& Direction)
{
	if (MovementComponent != nullptr && GetLocalRole() == ROLE_Authority)
	{
		LaunchVelocity = MovementComponent->InitialSpeed * Direction;

		OnRep_LaunchVelocity();
	}
}

void AProjectileBase::SetOwnerController(AController* NewController)
{
	OwnerController = NewController;
}

void AProjectileBase::SetOwnerCharacter(AActor* NewActor)
{
	OwnerCharacter = Cast<AStandardModePlayerCharacter>(NewActor);

	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed cast %s to AStandardModePlayerCharacter."), *NewActor->GetName());
	}
}


