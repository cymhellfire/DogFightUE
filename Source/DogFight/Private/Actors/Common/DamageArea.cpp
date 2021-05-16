// Dog Fight Game Code By CYM.


#include "Actors/Common/DamageArea.h"
#include "DamageStructures.h"
#include "Actors/Interfaces/DamageableActorInterface.h"

// Sets default values
ADamageArea::ADamageArea()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ADamageArea::InitializeDamageArea(float InDamage, float InDamageRadius, TSubclassOf<UDamageType> InDamageType)
{
	Damage = InDamage;
	DamageRadius = InDamageRadius;
	DamageType = InDamageType;
}

void ADamageArea::SetRelationship(AController* NewOwnerController, AActor* InInstigator)
{
	OwnerController = NewOwnerController;
	DamageInstigator = InInstigator;
}

void ADamageArea::SetStrengthCost(float InStrengthCost)
{
	StrengthCost = InStrengthCost;
}

void ADamageArea::Activate(float Delay /*= 0*/)
{
	if (Delay > 0.f)
	{
		GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &ADamageArea::OnDelayTimerExpired, Delay);
	}
	else
	{
		ApplyDamage();
	}
}

void ADamageArea::ApplyDamage()
{
	if (FMath::Abs(DamageRadius) < 0.01f)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	GetWorld()->OverlapMultiByProfile(OverlapResults, GetActorLocation(), GetActorRotation().Quaternion(),
		FName(TEXT("Projectile")), FCollisionShape::MakeSphere(DamageRadius));

	TArray<AActor*> DamagedActorList;
	for (FOverlapResult Result : OverlapResults)
	{
		if (AActor* TargetActor = Result.GetActor())
		{
			if (!DamagedActorList.Contains(TargetActor))
			{
				if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
				{
					DamageableActor->ApplyDamage(FDamageStruct{Damage,StrengthCost}, FDamageEvent{DamageType}, OwnerController, DamageInstigator);
				}
				DamagedActorList.Add(TargetActor);
			}
		}
	}

	OnDamageAreaFinished.Broadcast(this);

	// Self-destruct
	ConditionalBeginDestroy();
}

void ADamageArea::OnDelayTimerExpired()
{
	if (DelayTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DelayTimerHandle);
	}

	ApplyDamage();
}
