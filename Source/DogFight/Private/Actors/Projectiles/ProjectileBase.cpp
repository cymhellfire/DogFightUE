// Dog Fight Game Code By CYM.


#include "Actors/Projectiles/ProjectileBase.h"

#include "Card/GameCardTypes.h"
#include "Game/DogFightGameModeBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Actors/Vfx/VfxBase.h"
#include "Actors/Shield/ShieldBase.h"
#include "Actors/Managers/ShieldManager.h"

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
	CollisionComponent->SetCollisionProfileName(FName(TEXT("Projectile")));
	RootComponent = CollisionComponent;

	// Create Projectile Movement Component
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
	MovementComponent->bRotationFollowsVelocity = true;

	// Create Audio Component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);

	bIsAlive = true;
	DeadOnHit = true;
	DecayDuration = 0;
	Lifetime = 5.f;
	AlignVfxWithHitNormal = false;
	bIgnoreOwnerCollisionAtStart = true;

	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Actor.Projectile"))));
}

void AProjectileBase::GetGameplayTags(FGameplayTagContainer& OutGameplayTags)
{
	OutGameplayTags = GameplayTags;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Play the audio clip as default
	AudioComponent->Play();

	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &AProjectileBase::OnLifeTimerExpired, Lifetime);
}

void AProjectileBase::SetupShield()
{
	if (GetNetMode() == NM_Client)
		return;

	if (ADogFightGameModeBase* GameModeBase = Cast<ADogFightGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		if (AShieldManager* ShieldManager = GameModeBase->GetShieldManager())
		{
			TArray<AShieldBase*> ShieldList = ShieldManager->GetAllShield();
			for (AShieldBase* Shield : ShieldList)
			{
				// Ignore the shield collision based on shield type
				if (!Shield->CheckShouldBlockProjectile(this))
				{
					MulticastIgnoreActorWhileMoving(Shield, true);
				}
			}

			// Register callback
			ShieldManager->OnShieldRegistered.AddDynamic(this, &AProjectileBase::OnShieldRegistered);
			ShieldManager->OnShieldUnregistered.AddDynamic(this, &AProjectileBase::OnShieldUnregistered);
		}
	}
}

bool AProjectileBase::CheckDeadOnHitCondition(AActor* OtherActor)
{
	if (!DeadOnHit)
	{
		return false;
	}

	// Dead if tag query is empty
	if (HitDeadTagQuery.IsEmpty())
	{
		return true;
	}

	if (IGameplayTagsActorInterface* GameplayTagsActor = Cast<IGameplayTagsActorInterface>(OtherActor))
	{
		FGameplayTagContainer OtherActorGameplayTags;
		GameplayTagsActor->GetGameplayTags(OtherActorGameplayTags);

		return HitDeadTagQuery.Matches(OtherActorGameplayTags);
	}

	return true;
}

void AProjectileBase::MulticastIgnoreActorWhileMoving_Implementation(AActor* Target, bool bShouldIgnore)
{
	// Update collision
	CollisionComponent->IgnoreActorWhenMoving(Target, bShouldIgnore);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Record hit actor on server
	if (OtherActor != nullptr && GetLocalRole() == ROLE_Authority)
	{
		HitActor = OtherActor;
	}

	if (CheckDeadOnHitCondition(OtherActor))
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

void AProjectileBase::OnLifeTimerExpired()
{
	GetWorldTimerManager().ClearTimer(LifeTimerHandle);

	Dead();
}

void AProjectileBase::OnDecayTimerFinished()
{
	// Clear timer handle
	GetWorldTimerManager().ClearTimer(DecayTimerHandle);

	// Destroy this projectile
	// TODO: Pool projectiles in the future
	Destroy();
}

void AProjectileBase::OnShieldRegistered(AShieldBase* NewShield)
{
	if (ADogFightGameModeBase* GameModeBase = Cast<ADogFightGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		if (GameModeBase->GetPlayersRelation(OwnerController, NewShield->OwnerController) == EPlayerRelation::PR_Ally)
		{
			MulticastIgnoreActorWhileMoving(NewShield, true);
		}
	}
}

void AProjectileBase::OnShieldUnregistered(AShieldBase* Shield)
{
	if (ADogFightGameModeBase* GameModeBase = Cast<ADogFightGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		if (GameModeBase->GetPlayersRelation(OwnerController, Shield->OwnerController) == EPlayerRelation::PR_Ally)
		{
			MulticastIgnoreActorWhileMoving(Shield, false);
		}
	}
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OwnerCharacter == nullptr)
		return;

	// Recover collision of owner character after end overlap
	if (OtherActor == OwnerCharacter && bIgnoreOwnerCollisionAtStart)
	{
		MulticastIgnoreActorWhileMoving(OtherActor, false);
	}
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
	if (!bIsAlive)
	{
		return;
	}

	bIsAlive = false;
	// Clear timer
	if (LifeTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(LifeTimerHandle);
	}

	// Stop movement
	if (MovementComponent != nullptr)
	{
		MovementComponent->StopMovementImmediately();
	}

	// Damage target
	if (FMath::Abs(DamageRadius) < 0.01f)
	{
		if (HitActor != nullptr)
		{
			HitActor->TakeDamage(BaseDamage, FDamageEvent{DamageType}, OwnerController, this);
		}
	}
	else
	{
		TArray<FOverlapResult> OverlapList;
		GetWorld()->OverlapMultiByChannel(OverlapList, GetActorLocation(), GetActorRotation().Quaternion(),
			ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(DamageRadius));

		TArray<AActor*> DamagedActorList;
		for (FOverlapResult Result : OverlapList)
		{
			if (AActor* TargetActor = Result.GetActor())
			{
				// Ensure one actor only be damaged once
				if (!DamagedActorList.Contains(TargetActor))
				{
					TargetActor->TakeDamage(BaseDamage, FDamageEvent{DamageType}, OwnerController, this);
					DamagedActorList.Add(TargetActor);
				}
			}
		}
	}

	// Broadcast the event
	OnProjectileDead.Broadcast(this);

	// Disable collision
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Stop the audio
	if (AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}

	// Spawn Dead Vfx
	if (IsValid(VfxOnDead))
	{
		AVfxBase* Vfx = GetWorld()->SpawnActor<AVfxBase>(VfxOnDead);
		// Set owner
		Vfx->OwnerController = OwnerController;

		// Synchronize Vfx location with projectile
		Vfx->SetActorLocation(GetActorLocation());
		if (AlignVfxWithHitNormal)
		{
			
		}
	}

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

void AProjectileBase::BeginDestroy()
{
	Super::BeginDestroy();

	// Unregister callback
	if (GetNetMode() != NM_Client)
	{
		if (UWorld* MyWorld = GetWorld())
		{
			if (ADogFightGameModeBase* GameModeBase = Cast<ADogFightGameModeBase>(MyWorld->GetAuthGameMode()))
			{
				if (AShieldManager* ShieldManager = GameModeBase->GetShieldManager())
				{
					ShieldManager->OnShieldRegistered.RemoveDynamic(this, &AProjectileBase::OnShieldRegistered);
					ShieldManager->OnShieldUnregistered.RemoveDynamic(this, &AProjectileBase::OnShieldUnregistered);
				}
			}
		}
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

void AProjectileBase::SetDamage(float NewDamage)
{
	if (GetNetMode() != NM_Client)
	{
		BaseDamage = NewDamage;
	}
}

void AProjectileBase::SetDamageRadius(float NewDamageRadius)
{
	if (GetNetMode() != NM_Client)
	{
		DamageRadius = NewDamageRadius;
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

void AProjectileBase::SetIgnoreCollisionAtStart(bool bIgnore)
{
	bIgnoreOwnerCollisionAtStart = bIgnore;
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

	// Check all shield
	SetupShield();
}

void AProjectileBase::SetOwnerCharacter(AActor* NewActor)
{
	if (bIgnoreOwnerCollisionAtStart)
	{
		// Ignore the owner collision at start to avoid hit self
		MulticastIgnoreActorWhileMoving(NewActor, true);
	}

	OwnerCharacter = Cast<AStandardModePlayerCharacter>(NewActor);

	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed cast %s to AStandardModePlayerCharacter."), *NewActor->GetName());
	}
}


