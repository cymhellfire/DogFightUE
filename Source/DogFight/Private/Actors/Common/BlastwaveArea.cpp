// Dog Fight Game Code By CYM.


#include "Actors/Common/BlastwaveArea.h"
#include "Actors/Common/BlastwaveAreaSettings.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/StandardModePlayerCharacter.h"

void FBlastwaveAffectComponent::ApplyBlastForce() const
{
	AffectComponent->AddImpulse(BlastForce);
}

void FBlastwaveAffectCharacter::ApplyBlastForce() const
{
	AffectCharacter->TakeStrengthCost(StrengthCost, BlastForce);
}

// Sets default values
ABlastwaveArea::ABlastwaveArea()
{
	// Create sphere collider
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BlastwaveSphere"));
	SphereComponent->SetCollisionProfileName(FName(TEXT("OverlapAll")));
	SphereComponent->SetSphereRadius(100.f);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlastwaveArea::OnActorStartOverlapped);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABlastwaveArea::OnActorEndOverlapped);
	RootComponent = SphereComponent;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	bActive = false;
	ActiveTimer = 0.f;
}

void ABlastwaveArea::InitializeBlastwaveArea(FBlastwaveAreaSettings NewSettings)
{
	BlastForceSize = NewSettings.BlastForceSize;
	BlastForceMaxRadius = NewSettings.BlastMaxRadius;
	BlastForceMinRatio = NewSettings.BlastForceMinRatio;
	BlastForceExpandSpeed = NewSettings.BlastForceExpandSpeed;
	RaiseForceRatio = NewSettings.RaiseForceRatio;
	StrengthCost = NewSettings.StrengthCost;
	BlastForceFalloffCurve = NewSettings.BlastForceFalloffCurve;

	if (USphereComponent* SphereComponent = Cast<USphereComponent>(RootComponent))
	{
		SphereComponent->SetSphereRadius(BlastForceMaxRadius);
	}
}

void ABlastwaveArea::SetIgnoreActors(TArray<AActor*> Actors)
{
	IgnoreActors = Actors;

	FilterOverlappedRecord();
}

void ABlastwaveArea::Active()
{
	// Process all overlapped components
	for (UPrimitiveComponent* PrimitiveComponent : AffectComponentList)
	{
		// Skip component not movable
		if (PrimitiveComponent->Mobility != EComponentMobility::Movable || !PrimitiveComponent->IsSimulatingPhysics())
			continue;

		const TTuple<FVector, float> Result = CalculateBlastForceSize(PrimitiveComponent->GetComponentLocation());

		FBlastwaveAffectComponent NewComponent;
		NewComponent.AffectComponent = PrimitiveComponent;
		NewComponent.BlastForce = Result.Key;
		NewComponent.Delay = Result.Value;
		AffectComponentQueue.Add(NewComponent);
	}

	// Sort in descending order
	AffectComponentQueue.Sort([](const FBlastwaveAffectComponent& A, const FBlastwaveAffectComponent& B)
	{
		return A.Delay > B.Delay;
	});

	for (TTuple<AStandardModePlayerCharacter*, int32> Record : AffectCharacterMap)
	{
		const TTuple<FVector, float> Result = CalculateBlastForceSize(Record.Key->GetActorLocation());

		FBlastwaveAffectCharacter NewCharacter;
		NewCharacter.AffectCharacter = Record.Key;
		NewCharacter.BlastForce = Result.Key;
		NewCharacter.Delay = Result.Value;
		NewCharacter.StrengthCost = StrengthCost;
		AffectCharacterQueue.Add(NewCharacter);
	}

	// Sort in descending order
	AffectCharacterQueue.Sort([](const FBlastwaveAffectCharacter& A, const FBlastwaveAffectCharacter& B)
	{
		return A.Delay > B.Delay;
	});

	bActive = true;
}

// Called when the game starts or when spawned
void ABlastwaveArea::BeginPlay()
{
	Super::BeginPlay();

	// Initialize sphere radius on client side
	if (GetNetMode() == NM_Client)
	{
		OnRep_BlastForceMaxRadius();
	}
}

TTuple<FVector, float> ABlastwaveArea::CalculateBlastForceSize(FVector TargetPos) const
{
	const FVector BlastwaveCenter = GetActorLocation();
	const FVector BlastForceDir = TargetPos - BlastwaveCenter;
	const float CharacterDist = BlastForceDir.Size();
	float ForceRatio = 0.f;

	// Curve has higher priority
	if (IsValid(BlastForceFalloffCurve))
	{
		ForceRatio = BlastForceFalloffCurve->GetFloatValue(CharacterDist / BlastForceMaxRadius);
	}
	else
	{
		ForceRatio = FMath::Clamp<float>(1 - CharacterDist / BlastForceMaxRadius, BlastForceMinRatio, 1.f);
	}

	const FVector BlastForce = (BlastForceDir.GetSafeNormal() + FVector::UpVector * RaiseForceRatio) * BlastForceSize * ForceRatio;
	const float BlastDelay = BlastForceExpandSpeed > 0.01f ? CharacterDist / BlastForceExpandSpeed : 0.f;

	return TTuple<FVector, float>(BlastForce, BlastDelay);
}

void ABlastwaveArea::FilterOverlappedRecord()
{
	for (int32 Index = AffectComponentList.Num() - 1; Index >= 0; --Index)
	{
		if (IgnoreActors.Contains(AffectComponentList[Index]->GetOwner()))
		{
			AffectComponentList.RemoveAt(Index);
		}
	}

	for (TTuple<AStandardModePlayerCharacter*, int32> Record : AffectCharacterMap)
	{
		if (IgnoreActors.Contains(Record.Key))
		{
			AffectCharacterMap.Remove(Record.Key);
		}
	}
}

void ABlastwaveArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bActive)
		return;

	ActiveTimer += DeltaTime;

	// Apply blast force if exceed delay time
	for (int32 Index = AffectComponentQueue.Num() - 1; Index >= 0; --Index)
	{
		if (AffectComponentQueue[Index].Delay <= ActiveTimer)
		{
			// Apply force and remove record
			AffectComponentQueue[Index].ApplyBlastForce();
			AffectComponentQueue.RemoveAt(Index);
		}
		else
		{
			break;
		}
	}

	for (int32 Index = AffectCharacterQueue.Num() - 1; Index >= 0; --Index)
	{
		if (AffectCharacterQueue[Index].Delay <= ActiveTimer)
		{
			AffectCharacterQueue[Index].ApplyBlastForce();
			AffectCharacterQueue.RemoveAt(Index);
		}
		else
		{
			break;
		}
	}

	// Destroy this actor if no target left
	if (AffectComponentQueue.Num() == 0 && AffectCharacterQueue.Num() == 0)
	{
		bActive = false;
	}
}

void ABlastwaveArea::MulticastActive_Implementation()
{
	Active();
}

void ABlastwaveArea::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlastwaveArea, BlastForceSize);
	DOREPLIFETIME(ABlastwaveArea, BlastForceMinRatio);
	DOREPLIFETIME(ABlastwaveArea, BlastForceMaxRadius);
	DOREPLIFETIME(ABlastwaveArea, BlastForceExpandSpeed);
	DOREPLIFETIME(ABlastwaveArea, RaiseForceRatio);
	DOREPLIFETIME(ABlastwaveArea, BlastForceFalloffCurve);
}

void ABlastwaveArea::OnActorStartOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// Skip ignore actors
	if (IgnoreActors.Contains(OtherActor))
		return;

	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(OtherActor))
	{
		if (!AffectCharacterMap.Contains(StandardModePlayerCharacter))
		{
			AffectCharacterMap.Add(StandardModePlayerCharacter, 1);
		}
		else
		{
			AffectCharacterMap[StandardModePlayerCharacter]++;
		}
	}
	else
	{
		if (!AffectComponentList.Contains(OtherComp))
		{
			AffectComponentList.Add(OtherComp);
		}
	}
}

void ABlastwaveArea::OnActorEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Skip ignore actors
	if (IgnoreActors.Contains(OtherActor))
		return;

	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(OtherActor))
	{
		if (AffectCharacterMap.Contains(StandardModePlayerCharacter))
		{
			AffectCharacterMap[StandardModePlayerCharacter]--;
			if (AffectCharacterMap[StandardModePlayerCharacter] <= 0)
			{
				AffectCharacterMap.Remove(StandardModePlayerCharacter);
			}
		}
	}
	else
	{
		if (AffectComponentList.Contains(OtherComp))
		{
			AffectComponentList.Remove(OtherComp);
		}
	}
}

void ABlastwaveArea::OnRep_BlastForceMaxRadius()
{
	if (USphereComponent* SphereComponent = Cast<USphereComponent>(RootComponent))
	{
		SphereComponent->SetSphereRadius(BlastForceMaxRadius);
	}
}

