// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionFireProjectile.h"
#include "Card/CardBase.h"
#include "Actors/Interfaces/GameProjectileInterface.h"
#include "GameFramework/Character.h"

UInstructionFireProjectile::UInstructionFireProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("FireProjectile");
	ProjectileSpawnDistance = 50.f;
	ProjectileSpawnHeight = 100.f;
	Damage = FUpgradableIntProperty(5, FString(TEXT("Property_Damage")), ECardDisplayInfoLocType::ILT_Card);
	MuzzleSpeed = FUpgradableIntProperty(1000, FString(TEXT("Property_MuzzleSpeed")), ECardDisplayInfoLocType::ILT_Card);
	DamageRadius = FUpgradableIntProperty(0, FString(TEXT("Property_DamageRadius")), ECardDisplayInfoLocType::ILT_Card);
	StrengthCost = FUpgradableIntProperty(0, FString(TEXT("Property_StrengthCost")), ECardDisplayInfoLocType::ILT_Card);

	// Turn off the auto finish since we need wait for projectiles dead
	bAutoFinish = false;
}

bool UInstructionFireProjectile::HandleActorTarget(AActor* Target)
{
	const bool Result = Super::HandleActorTarget(Target);

	if (!Result)
	{
		return false;
	}

	// Skip once broadcast since the jump invoke below
	bSkipOneBroadcast = true;

	if (IsValid(Target))
	{
		HandlePositionTarget(Target->GetActorLocation());
	}
	return true;
}

void UInstructionFireProjectile::HandlePositionTarget(FVector Position)
{
	Super::HandlePositionTarget(Position);

	if (IsValid(ProjectileClass))
	{
		FVector SpawnDirection = (Position - GetOwnerControlledPawn()->GetActorLocation());
		SpawnDirection.Normalize();

		const FRotator SpawnRotation = SpawnDirection.Rotation();
		// Calculate the spawn position
		FVector SpawnPosition;
		if (SpawnLocationType == ESpawnLocationType::SLT_Offset)
		{
			SpawnPosition = GetOwnerControlledPawn()->GetActorLocation();
			SpawnPosition += SpawnRotation.RotateVector(FVector(ProjectileSpawnDistance, 0, ProjectileSpawnHeight));
		}
		else if (SpawnLocationType == ESpawnLocationType::SLT_Socket)
		{
			APawn* OwnerPawn = GetOwnerControlledPawn();
			if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
			{
				USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
				SpawnPosition = MeshComponent->GetSocketLocation(SocketName);
			}
		}

		// Calculate launch direction
		FVector FireDirection = Position - SpawnPosition;
		FireDirection.Normalize();

		SpawnProjectileAndLaunch(SpawnPosition, SpawnRotation, FireDirection, SpawnDirection == FVector::ZeroVector);
	}
}

void UInstructionFireProjectile::HandleDirectionTarget(FVector Direction)
{
	Super::HandleDirectionTarget(Direction);

	if (IsValid(ProjectileClass))
	{
		FVector FireDirection = Direction.GetSafeNormal();

		const FRotator SpawnRotation = FireDirection.Rotation();
		// Calculate spawn position
		FVector SpawnPosition;
		if (SpawnLocationType == ESpawnLocationType::SLT_Offset)
		{
			SpawnPosition= GetOwnerControlledPawn()->GetActorLocation();
			SpawnPosition += SpawnRotation.RotateVector(FVector(ProjectileSpawnDistance, 0, ProjectileSpawnHeight));
		}
		else if (SpawnLocationType == ESpawnLocationType::SLT_Socket)
		{
			APawn* OwnerPawn = GetOwnerControlledPawn();
			if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
			{
				USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
				SpawnPosition = MeshComponent->GetSocketLocation(SocketName);
			}
		}

		SpawnProjectileAndLaunch(SpawnPosition, SpawnRotation, FireDirection, false);
	}
}

void UInstructionFireProjectile::OnProjectileDead(AActor* Projectile)
{
	check(Projectile);

	IGameProjectileInterface* GameProjectileInterface = Cast<IGameProjectileInterface>(Projectile);
	// Remove from list
	if (ProjectileInstanceList.Contains(GameProjectileInterface))
	{
		GameProjectileInterface->GetProjectileDeadDelegate().RemoveDynamic(this, &UInstructionFireProjectile::OnProjectileDead);
		ProjectileInstanceList.Remove(GameProjectileInterface);

		// Invoke Finish() after all projectiles are dead
		if (ProjectileInstanceList.Num() == 0)
		{
			Finish();
		}
	}
}

void UInstructionFireProjectile::SpawnProjectileAndLaunch(FVector Position, FRotator Rotation, FVector FireDirection, bool bMayHitSelf)
{
	IGameProjectileInterface* Projectile = GetWorld()->SpawnActor<IGameProjectileInterface>(ProjectileClass, Position, Rotation);
	if (Projectile != nullptr)
	{
		Projectile->GetProjectileDeadDelegate().AddDynamic(this, &UInstructionFireProjectile::OnProjectileDead);
		Projectile->SetIgnoreCollisionAtStart(!bMayHitSelf);
		Projectile->SetOwnerController(GetOwnerCard()->GetOwnerPlayerController());
		Projectile->SetOwnerCharacter(GetOwnerControlledPawn());
		Projectile->SetDamage(Damage.GetValue());
		Projectile->SetDamageRadius(DamageRadius.GetValue());
		Projectile->SetStrengthCost(StrengthCost.GetValue());
		Projectile->SetInitialSpeed(MuzzleSpeed.GetValue());
		Projectile->LaunchAtDirection(FireDirection);

		// Record the new instance to list
		ProjectileInstanceList.Add(Projectile);
	}
}


