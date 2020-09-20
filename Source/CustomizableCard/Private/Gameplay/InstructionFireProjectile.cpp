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

		const FRotator DirRotator = FRotator::MakeFromEuler(FireDirection);
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

		const FRotator DirRotator = FRotator::MakeFromEuler(FireDirection);
		// Calculate spawn position
		FVector SpawnPosition = GetOwnerControlledPawn()->GetActorLocation();
		SpawnPosition += DirRotator.RotateVector(FVector(ProjectileSpawnDistance, 0, ProjectileSpawnHeight));

		SpawnProjectileAndLaunch(SpawnPosition, DirRotator, FireDirection);
	}
}

void UInstructionFireProjectile::SpawnProjectileAndLaunch(FVector Position, FRotator Rotation, FVector FireDirection)
{
	ProjectileInstance = GetWorld()->SpawnActor<IGameProjectileInterface>(ProjectileClass, Position, Rotation);
	if (ProjectileInstance != nullptr)
	{
		ProjectileInstance->SetInitialSpeed(MuzzleSpeed);
		ProjectileInstance->LaunchAtDirection(FireDirection);
	}
}

APawn* UInstructionFireProjectile::GetOwnerControlledPawn() const
{
	APlayerController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	if (IGameCardUserPlayerControllerInterface* CardUserController = Cast<IGameCardUserPlayerControllerInterface>(PlayerController))
	{
		return CardUserController->GetActualPawn();
	}

	return nullptr;
}
