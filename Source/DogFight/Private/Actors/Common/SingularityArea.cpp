// Dog Fight Game Code By CYM.


#include "Actors/Common/SingularityArea.h"
#include "Game/DogFightDamageType.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ASingularityArea::ASingularityArea()
{
	// Create sphere collider
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SingularitySphere"));
	SphereComponent->SetCollisionProfileName(FName(TEXT("OverlapAll")));
	SphereComponent->SetSphereRadius(100.f);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASingularityArea::OnActorStartOverlapped);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASingularityArea::OnActorEndOverlapped);
	RootComponent = SphereComponent;

	PrimaryActorTick.bCanEverTick = true;

	RaiseForceRatio = 0.5f;
	MinDragForceRatio = 0.f;
}

void ASingularityArea::InitializeSingularityArea(float InDragForceSize, float MaxDistance, float MinDistance, float InDuration, float InMinDragForceRatio)
{
	DragForceSize = InDragForceSize;
	DragForceMaxDistance = MaxDistance;
	DragForceMinDistance = MinDistance;
	SingularityAreaDuration = InDuration;
	MinDragForceRatio = InMinDragForceRatio;

	if (USphereComponent* SphereComponent = Cast<USphereComponent>(RootComponent))
	{
		SphereComponent->SetSphereRadius(DragForceMaxDistance);
	}
}

void ASingularityArea::SetupStrengthCost(int32 InStrengthCostPerTick, float InTickInterval)
{
	StrengthCostPerTick = InStrengthCostPerTick;
	StrengthCostTickInterval = InTickInterval;
}

void ASingularityArea::SetupDamageRelative(TSubclassOf<UDamageType> InDamageType, AActor* InDamageInstigator)
{
	DamageType = InDamageType;
	DragInstigator = InDamageInstigator;
}

void ASingularityArea::Active()
{
	bActive = true;

	if (StrengthCostPerTick > 0 && StrengthCostTickInterval != 0.f && IsValid(DamageType))
	{
		bCostStrength = true;
		StrengthCostTickTimer = 0.f;
	}
}

void ASingularityArea::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bActive)
	{
		return;
	}

	// Add force to normal component
	for (UPrimitiveComponent* Component : DraggingComponentList)
	{
		if (Component->Mobility != EComponentMobility::Movable || !Component->IsSimulatingPhysics())
			continue;

		const FVector DragForce = CalculateDragForce(Component->GetComponentLocation());
		Component->AddForce(DragForce);
	}

	// Add force to ragdolls
	for (TTuple<AStandardModePlayerCharacter*, int> DraggingRecord : DraggingCharacterMap)
	{
		if (DraggingRecord.Key->IsRagdollActive())
		{
			DraggingRecord.Key->AddForceToAllRagdollBodies(CalculateDragForce(DraggingRecord.Key->GetActorLocation()));
		}
	}

	GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Yellow, FString::Printf(TEXT("Dragging component count: %d"), DraggingComponentList.Num()));

	// Tick for strength cost
	if (bCostStrength)
	{
		if (StrengthCostTickTimer >= StrengthCostTickInterval)
		{
			// Update dragging list
			if (DraggingCharacterMap.Num() > 0)
			{
				for (AStandardModePlayerCharacter* Character : CharacterToRemove)
				{
					DraggingCharacterMap.Remove(Character);
				}

				CharacterToRemove.Empty();
			}

			for (TTuple<AStandardModePlayerCharacter*, int> DraggingRecord : DraggingCharacterMap)
			{
				if (!DraggingRecord.Key->IsRagdollActive())
				{
					DraggingRecord.Key->TakeStrengthCost(StrengthCostPerTick, Cast<UDogFightDamageType>(DamageType->GetDefaultObject()), DragInstigator);
				}
			}
			StrengthCostTickTimer = 0.f;
		}
		StrengthCostTickTimer += DeltaSeconds;
	}

	SingularityAreaTimer += DeltaSeconds;
	if (SingularityAreaTimer >= SingularityAreaDuration)
	{
		Destroy();
	}
}

FVector ASingularityArea::CalculateDragForce(FVector Pos)
{
	const FVector Center = GetActorLocation();
	FVector ForceDirection = Center - Pos;
	const float Distance = ForceDirection.Size();

	if (Distance <= DragForceMinDistance)
	{
		return FVector::ZeroVector;
	}

	const float ForceRatio = FMath::Clamp(1 - (Distance - DragForceMinDistance) / DragForceMaxDistance, MinDragForceRatio, 1.f);
	ForceDirection.Normalize();

	const FVector Force = (ForceDirection + FVector::UpVector * RaiseForceRatio) * ForceRatio * DragForceSize;
	DrawDebugLine(GetWorld(), Pos, Pos + Force, FColor::Yellow);

	return Force;
}

void ASingularityArea::OnActorStartOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(OtherActor))
	{
		if (!DraggingCharacterMap.Contains(StandardModePlayerCharacter))
		{
			DraggingCharacterMap.Add(StandardModePlayerCharacter, 1);
		}
		else
		{
			DraggingCharacterMap[StandardModePlayerCharacter]++;
		}
	}
	else
	{
		if (!DraggingComponentList.Contains(OtherComp))
		{
			DraggingComponentList.Add(OtherComp);
		}
	}
}

void ASingularityArea::OnActorEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(OtherActor))
	{
		if (DraggingCharacterMap.Contains(StandardModePlayerCharacter))
		{
			DraggingCharacterMap[StandardModePlayerCharacter]--;
			if (DraggingCharacterMap[StandardModePlayerCharacter] <= 0)
			{
				CharacterToRemove.Add(StandardModePlayerCharacter);
			}
		}
	}
	else
	{
		if (DraggingComponentList.Contains(OtherComp))
		{
			DraggingComponentList.Remove(OtherComp);
		}
	}
}
