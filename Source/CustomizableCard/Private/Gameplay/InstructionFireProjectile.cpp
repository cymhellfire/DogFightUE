// Dog Fight Game Code By CYM.


#include "InstructionFireProjectile.h"
#include "CardBase.h"
#include "GameCardUserPlayerControllerInterface.h"
#include "GameProjectileInterface.h"

UInstructionFireProjectile::UInstructionFireProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("FireProjectile");
	MuzzleSpeed = 1000.f;
	ProjectileSpawnDistance = 50.f;
	ProjectileSpawnHeight = 100.f;

	// Turn off the auto finish since we need wait for projectiles dead
	bAutoFinish = false;
}

void UInstructionFireProjectile::HandleActorTarget(AActor* Target)
{
	if (IsValid(Target))
	{
		HandlePositionTarget(Target->GetActorLocation());
	}
}

void UInstructionFireProjectile::HandlePositionTarget(FVector Position)
{
	if (IsValid(ProjectileClass))
	{
		FVector FireDirection = (Position - GetOwnerControlledPawn()->GetActorLocation());
		FireDirection.Normalize();

		const FRotator DirRotator = FireDirection.Rotation();
		// Calculate the spawn position
		FVector SpawnPosition = GetOwnerControlledPawn()->GetActorLocation();
		SpawnPosition += DirRotator.RotateVector(FVector(ProjectileSpawnDistance, 0, ProjectileSpawnHeight));

		SpawnProjectileAndLaunch(SpawnPosition, DirRotator, FireDirection);
	}
}

void UInstructionFireProjectile::HandleDirectionTarget(FVector Direction)
{
	if (IsValid(ProjectileClass))
	{
		FVector FireDirection = Direction.GetSafeNormal();

		const FRotator DirRotator = FireDirection.Rotation();
		// Calculate spawn position
		FVector SpawnPosition = GetOwnerControlledPawn()->GetActorLocation();
		SpawnPosition += DirRotator.RotateVector(FVector(ProjectileSpawnDistance, 0, ProjectileSpawnHeight));

		SpawnProjectileAndLaunch(SpawnPosition, DirRotator, FireDirection);
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

void UInstructionFireProjectile::SpawnProjectileAndLaunch(FVector Position, FRotator Rotation, FVector FireDirection)
{
	IGameProjectileInterface* Projectile = GetWorld()->SpawnActor<IGameProjectileInterface>(ProjectileClass, Position, Rotation);
	if (Projectile != nullptr)
	{
		Projectile->GetProjectileDeadDelegate().AddDynamic(this, &UInstructionFireProjectile::OnProjectileDead);
		Projectile->SetOwnerController(GetOwnerCard()->GetOwnerPlayerController());
		Projectile->SetOwnerCharacter(GetOwnerControlledPawn());
		Projectile->SetInitialSpeed(MuzzleSpeed);
		Projectile->LaunchAtDirection(FireDirection);
	}

	// Record the new instance to list
	ProjectileInstanceList.Add(Projectile);
}

APawn* UInstructionFireProjectile::GetOwnerControlledPawn() const
{
	AController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	if (IGameCardUserPlayerControllerInterface* CardUserController = Cast<IGameCardUserPlayerControllerInterface>(PlayerController))
	{
		return CardUserController->GetActualPawn();
	}

	return nullptr;
}
