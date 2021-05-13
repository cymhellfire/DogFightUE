// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionSpawnProjectileAt.h"
#include "Card/CardBase.h"

UInstructionSpawnProjectileAt::UInstructionSpawnProjectileAt(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("SpawnProjectileAt");
	Damage = FUpgradableIntProperty(5, FString(TEXT("Property_Damage")), ECardDisplayInfoLocType::ILT_Card);
	InitialSpeed = FUpgradableIntProperty(500, FString(TEXT("Property_MuzzleSpeed")), ECardDisplayInfoLocType::ILT_Card);
	DamageRadius = FUpgradableIntProperty(0, FString(TEXT("Property_DamageRadius")), ECardDisplayInfoLocType::ILT_Card);
	StrengthCost = FUpgradableIntProperty(0, FString(TEXT("Property_StrengthCost")), ECardDisplayInfoLocType::ILT_Card);

	bAutoFinish = false;
}

void UInstructionSpawnProjectileAt::HandlePositionTarget(FVector Position)
{
	Super::HandlePositionTarget(Position);

	if (IsValid(ProjectileClass))
	{
		FVector SpawnPosition = Position + SpawnPositionOffset;
		FRotator SpawnRotation = InitialFacingDirection.Rotation();

		SpawnProjectile(SpawnPosition, SpawnRotation);
	}
}

bool UInstructionSpawnProjectileAt::HandleActorTarget(AActor* Target)
{
	const bool Result = Super::HandleActorTarget(Target);
	if (!Result)
	{
		return false;
	}

	// Skip once broadcast since the jump invoke below
	bSkipOneBroadcast = true;

	HandlePositionTarget(Target->GetActorLocation());
	return true;
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
		Projectile->SetStrengthCost(StrengthCost.GetValue());
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

