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

	// Damage hit actor
	if (OtherActor != nullptr)
	{
		OtherActor->TakeDamage(BaseDamage, FDamageEvent(), OwnerController, OwnerCharacter);
	}

	if (DeadOnHit)
	{
		Dead();
	}
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::Dead()
{
	// Broadcast the event
	OnProjectileDead.Broadcast(this);

	// Destroy this projectile
	// TODO: Pool projectiles in the future
	Destroy();
}

void AProjectileBase::AdjustGravityScale(float NewGravityScale)
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->ProjectileGravityScale = NewGravityScale;
	}
}

void AProjectileBase::SetInitialSpeed(float NewSpeed)
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->InitialSpeed = NewSpeed;
	}
}

void AProjectileBase::SetMaxSpeed(float NewSpeed)
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->MaxSpeed = NewSpeed;
	}
}

void AProjectileBase::LaunchAtDirection(const FVector& Direction)
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->Velocity = MovementComponent->InitialSpeed * Direction;
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


