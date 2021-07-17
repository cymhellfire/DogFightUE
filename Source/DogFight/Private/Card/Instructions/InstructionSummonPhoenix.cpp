// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionSummonPhoenix.h"

#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Actors/Interfaces/GameProjectileInterface.h"
#include "Actors/Vfx/MoveToTargetVfx.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"
#include "GameFramework/PlayerState.h"

UInstructionSummonPhoenix::UInstructionSummonPhoenix(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("SummonPhoenix");
	TotalDamage = FUpgradableIntProperty(50, FString(TEXT("Property_TotalDamage")), ECardDisplayInfoLocType::ILT_Card);
	PhoenixTargetZOffset = 300.f;

	bAutoFinish = false;
	CameraFocusType = EInstructionCameraFocusType::ICFT_User;
	bForciblyCameraEventToOwner = true;
}

void UInstructionSummonPhoenix::ProcessTarget()
{
	if (OwnerCard == nullptr)
	{
		UE_LOG(LogGameCards, Error, TEXT("No OwnerCard assigned to this instruction."));
		return;
	}

	FString PlayerNameList;
	while (CurrentTargetIndex < OwnerCard->GetTargetInfoCount())
	{
		FCardInstructionTargetInfo TargetInfo = OwnerCard->GetTargetInfo(CurrentTargetIndex);

		// Skip non-actor targets
		if (TargetInfo.TargetType != ECardInstructionTargetType::Actor)
		{
			continue;
		}

		// Record target actor
		TargetActorList.Add(TargetInfo.ActorPtr);

		if (PlayerNameList.Len() != 0)
		{
			PlayerNameList.Append(", ");
		}

		if (APawn* TargetPawn = Cast<APawn>(TargetInfo.ActorPtr))
		{
			if (APlayerState* PlayerState = TargetPawn->GetPlayerState())
			{
				PlayerNameList.Appendf(TEXT("<PlayerName>%s</>"), *PlayerState->GetPlayerName());
			}
			else
			{
				PlayerNameList.Append(TargetInfo.ActorPtr->GetName());
			}
		}
		else
		{
			PlayerNameList.Append(TargetInfo.ActorPtr->GetName());
		}

		CurrentTargetIndex++;
	}

	if (IGameCardUserPlayerControllerInterface* UserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(OwnerCard->GetOwnerPlayerController()))
	{
		UserPlayerController->BroadcastCardTargetingResult(OwnerCard->GetCardDisplayInfo().GetCardNameText(),
			FText::FromString(PlayerNameList), ECardInstructionTargetType::Actor);
		bUseMessageSent = true;
	}

	// Calculate the phoenix flying destination
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetOwnerCard()->GetWorld()->GetAuthGameMode()))
	{
		PhoenixTargetPosition = StandardGameMode->GetCenterPointOfAllAlivePlayers();
	} 

	// Forcibly set the Z axis value based on user location
	const FVector UserLocation = GetOwnerControlledPawn()->GetActorLocation();
	PhoenixTargetPosition.Z = UserLocation.Z + PhoenixTargetZOffset;

	// Spawn phoenix vfx
	if (IsValid(PhoenixVfxClass))
	{
		AMoveToTargetVfx* NewVfx = OwnerCard->GetWorld()->SpawnActor<AMoveToTargetVfx>(PhoenixVfxClass, UserLocation, FRotator::ZeroRotator);
		NewVfx->SetTargetLocation(NewVfx->GetTransform().InverseTransformPosition(PhoenixTargetPosition));
	}

	GetWorld()->GetTimerManager().SetTimer(PhoenixTimerHandle, this, &UInstructionSummonPhoenix::OnPhoenixReachDestination, PhoenixFireSpawnDelay);
}

void UInstructionSummonPhoenix::OnPhoenixReachDestination()
{
	if (IsValid(ProjectileClass))
	{
		// Calculate damage based on target count
		float DealDamage = (float)TotalDamage.GetValue() / TargetActorList.Num();

		// Fire projectiles toward recorded targets
		for (int32 Index = 0; Index < TargetActorList.Num(); ++Index)
		{
			const FVector Direction = (TargetActorList[Index]->GetActorLocation() - PhoenixTargetPosition).GetSafeNormal();
			FVector SpawnLocation = PhoenixTargetPosition + Direction * ProjectileSpawnDistance;
			AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);

			if (IGameProjectileInterface* ProjectileInterface = Cast<IGameProjectileInterface>(Projectile))
			{
				for (AActor* Instance : ProjectileInstances)
				{
					if (IGameProjectileInterface* ExistProjectile = Cast<IGameProjectileInterface>(Instance))
					{
						ExistProjectile->SetIgnoreActor(Projectile);
					}
				}

				ProjectileInterface->GetProjectileDeadDelegate().AddDynamic(this, &UInstructionSummonPhoenix::OnProjectileDead);
				ProjectileInterface->SetOwnerController(GetOwnerCard()->GetOwnerPlayerController());
				ProjectileInterface->SetDamage(DealDamage);
				ProjectileInterface->SetInitialSpeed(ProjectileSpeed);
				ProjectileInterface->LaunchAtDirection((TargetActorList[Index]->GetActorLocation() - SpawnLocation).GetSafeNormal());
				ProjectileInterface->SetIgnoreActors(ProjectileInstances);

				ProjectileInstances.Add(Cast<AActor>(Projectile));
			}
			else
			{
				Projectile->Destroy();
			}
		}
	}
	else
	{
		Finish();
	}
}

void UInstructionSummonPhoenix::OnProjectileDead(AActor* Projectile)
{
	if (ProjectileInstances.Contains(Projectile))
	{
		if (IGameProjectileInterface* Interface = Cast<IGameProjectileInterface>(Projectile))
		{
			Interface->GetProjectileDeadDelegate().RemoveDynamic(this, &UInstructionSummonPhoenix::OnProjectileDead);
		}
		ProjectileInstances.Remove(Projectile);

		if (ProjectileInstances.Num() == 0)
		{
			Finish();
		}
	}
}


