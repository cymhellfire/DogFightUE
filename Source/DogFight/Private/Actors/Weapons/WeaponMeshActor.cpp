// Dog Fight Game Code By CYM.


#include "Actors/Weapons/WeaponMeshActor.h"
#include "GameFramework/Character.h"
#include "Pawns/StandardModePlayerCharacter.h"

// Sets default values
AWeaponMeshActor::AWeaponMeshActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponMeshActor::OnMeshComponentBeginOverlap);
}

void AWeaponMeshActor::SetWeaponMeshOwner(ACharacter* OwnerCharacter)
{
	WeaponOwner = OwnerCharacter;
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(OwnerCharacter))
	{
		WeaponOwnerController = StandardModePlayerCharacter->GetSupremeController();
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("[WeaponMeshActor] Failed to get supreme controller of weapon %s"), *GetName());
	}
}

void AWeaponMeshActor::SetDetectHit(bool bActivate)
{
	if (bActivate)
	{
		if (DefaultCollisionProfile.IsNone())
		{
			DefaultCollisionProfile = StaticMeshComponent->GetCollisionProfileName();
		}
		StaticMeshComponent->SetCollisionProfileName(HitDetectCollisionProfile);
	}
	else
	{
		if (!DefaultCollisionProfile.IsNone())
		{
			StaticMeshComponent->SetCollisionProfileName(DefaultCollisionProfile);
		}
	}
}

void AWeaponMeshActor::SetDamageRatio(float NewDamageRatio)
{
	DamageRatio = NewDamageRatio;
}

void AWeaponMeshActor::OnMeshComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Don't damage owner
	if (OtherActor == WeaponOwner)
	{
		return;
	}

	OtherActor->TakeDamage(WeaponDamage * DamageRatio, FDamageEvent{WeaponDamageType}, WeaponOwnerController, WeaponOwner);
}
