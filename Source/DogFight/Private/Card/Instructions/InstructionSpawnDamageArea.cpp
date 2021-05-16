// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionSpawnDamageArea.h"
#include "Actors/Vfx/VfxBase.h"
#include "Actors/Common/DamageArea.h"
#include "Actors/Common/SingularityArea.h"
#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"

UInstructionSpawnDamageArea::UInstructionSpawnDamageArea(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoFinish = false;

	Damage = FUpgradableIntProperty(0, FString(TEXT("Property_Damage")), ECardDisplayInfoLocType::ILT_Card);
	DamageRadius = FUpgradableIntProperty(0, FString(TEXT("Property_DamageRadius")), ECardDisplayInfoLocType::ILT_Card);
	StrengthCost = FUpgradableIntProperty(0, FString(TEXT("Property_StrengthCost")), ECardDisplayInfoLocType::ILT_Card);

	CameraFocusType = EInstructionCameraFocusType::ICFT_Target;
	bForciblyCameraEventToOwner = true;
}

void UInstructionSpawnDamageArea::HandlePositionTarget(FVector Position)
{
	Super::HandlePositionTarget(Position);

	// Spawn damage area
	ADamageArea* DamageArea = GetWorld()->SpawnActor<ADamageArea>(ADamageArea::StaticClass(), Position, FRotator::ZeroRotator);
	DamageArea->InitializeDamageArea(Damage.GetValue(), DamageRadius.GetValue(), DamageType);
	DamageArea->SetStrengthCost(StrengthCost.GetValue());
	if (AController* Controller = OwnerCard->GetOwnerPlayerController())
	{
		if (IGameCardUserPlayerControllerInterface* CardUserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(Controller))
		{
			DamageArea->SetRelationship(Controller, CardUserPlayerController->GetActualPawn());
		}
	}
	DamageArea->OnDamageAreaFinished.AddDynamic(this, &UInstructionSpawnDamageArea::OnDamageAreaFinished);
	DamageArea->Activate(DamageDelay);
	DamageAreaList.Add(DamageArea);

	// Spawn vfx
	if (IsValid(VfxClass))
	{
		GetWorld()->SpawnActor<AVfxBase>(VfxClass, Position, FRotator::ZeroRotator);
	}

	// Spawn singularity area
	if (bSpawnSingularityArea)
	{
		ASingularityArea* SingularityArea = GetWorld()->SpawnActor<ASingularityArea>(ASingularityArea::StaticClass(), Position, FRotator::ZeroRotator);
		SingularityArea->InitializeSingularityArea(SingularityDragForceSize, SingularityAreaMaxRadius, SingularityAreaMinRadius, SingularityDuration, DragForceMinRatio);
		SingularityArea->SetupStrengthCost(SingularityStrengthCostPerTick, SingularityStrengthCostTickInterval);
		if (AController* Controller = OwnerCard->GetOwnerPlayerController())
		{
			if (IGameCardUserPlayerControllerInterface* CardUserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(Controller))
			{
				SingularityArea->SetupDamageRelative(DamageType, CardUserPlayerController->GetActualPawn());
			}
		}
		UE_LOG(LogInit, Log, TEXT("Spawn SingularityArea at %s"), *SingularityArea->GetActorLocation().ToString()); 
		SingularityArea->Active();
	}
}

void UInstructionSpawnDamageArea::OnDamageAreaFinished(ADamageArea* DamageArea)
{
	DamageArea->OnDamageAreaFinished.RemoveDynamic(this, &UInstructionSpawnDamageArea::OnDamageAreaFinished);

	DamageAreaList.Remove(DamageArea);

	// Check instruction is finished
	if (DamageAreaList.Num() == 0)
	{
		Finish();
	}
}
