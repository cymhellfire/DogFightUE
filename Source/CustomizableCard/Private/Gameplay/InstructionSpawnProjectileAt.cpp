// Dog Fight Game Code By CYM.


#include "InstructionSpawnProjectileAt.h"
#include "CardBase.h"

UInstructionSpawnProjectileAt::UInstructionSpawnProjectileAt(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("SpawnProjectileAt");
	Damage = FUpgradableIntProperty(5, FString(TEXT("Property_Damage")), ECardDisplayInfoLocType::ILT_Card);
	InitialSpeed = FUpgradableIntProperty(500, FString(TEXT("Property_MuzzleSpeed")), ECardDisplayInfoLocType::ILT_Card);
	DamageRadius = FUpgradableIntProperty(0, FString(TEXT("Property_DamageRadius")), ECardDisplayInfoLocType::ILT_Card);

	bAutoFinish = false;
}

void UInstructionSpawnProjectileAt::HandlePositionTarget(FVector Position)
{
	if (IsValid(ProjectileClass))
	{
		FVector SpawnPosition = Position + SpawnPositionOffset;
		FRotator SpawnRotation = InitialFacingDirection.Rotation();

		SpawnProjectile(SpawnPosition, SpawnRotation);
	}
}

void UInstructionSpawnProjectileAt::HandleActorTarget(AActor* Target)
{
	HandlePositionTarget(Target->GetActorLocation());
}

void UInstructionSpawnProjectileAt::HandleDirectionTarget(FVector Direction)
{
	// Not support
}

void UInstructionSpawnProjectileAt::SpawnProjectile(FVector Position, FRotator Rotation)
{
	IGameProjectileInterface* Projectile = GetWorld()->SpawnActor<IGameProjectileInterface>(ProjectileClass, Position, Rotation);
	if (Projectile)
	{
		Projectile->GetProjectileDeadDelegate().AddDynamic(this, &UInstructionSpawnProjectileAt::OnProjectileDead);
		Projectile->SetOwnerController(GetOwnerCard()->GetOwnerPlayerController());
		Projectile->SetOwnerCharacter(GetOwnerControlledPawn());
		Projectile->SetInitialSpeed(InitialSpeed.GetValue());
		Projectile->SetDamage(Damage.GetValue());
		Projectile->SetDamageRadius(DamageRadius.GetValue());
		Projectile->LaunchAtDirection(InitialFacingDirection);
		
		// Record the new instance to list
		ProjectileInstances.Add(Projectile);
	}
}

void UInstructionSpawnProjectileAt::OnProjectileDead(AActor* Projectile)
{
	check(Projectile);

	IGameProjectileInterface* GameProjectileInterface = Cast<IGameProjectileInterface>(Projectile);
	// Remove from list
	if (ProjectileInstances.Contains(GameProjectileInterface))
	{
		GameProjectileInterface->GetProjectileDeadDelegate().RemoveDynamic(this, &UInstructionSpawnProjectileAt::OnProjectileDead);
		ProjectileInstances.Remove(GameProjectileInterface);

		if (ProjectileInstances.Num() == 0)
		{
			Finish();
		}
	}
}

