// Dog Fight Game Code By CYM.


#include "Actor/WeaponModelBase.h"

#include "GameFramework/Character.h"


// Sets default values
AWeaponModelBase::AWeaponModelBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initial values
	bAttackDetecting = true;
	bDetectComponentValid = false;
}

void AWeaponModelBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Unbind component
	UnbindAttackDetectComponent();
}

// Called when the game starts or when spawned
void AWeaponModelBase::BeginPlay()
{
	Super::BeginPlay();

	// Try to bind attack detect component
	UPrimitiveComponent* TargetComponent = nullptr;
	TInlineComponentArray<UPrimitiveComponent*> DetectComponents;
	GetComponents<UPrimitiveComponent>(DetectComponents);
	for (auto Candidate : DetectComponents)
	{
		if (Candidate->GetFName() == DetectComponentName)
		{
			TargetComponent = Candidate;
			break;
		}
	}

	if (IsValid(TargetComponent))
	{
		SetAttackDetectComponent(TargetComponent);
	}

	// Turn off attack detect as default
	SetAttackDetectEnable(false);
}

void AWeaponModelBase::SetAttackDetectEnable(bool bEnable)
{
	if (bAttackDetecting == bEnable)
	{
		return;
	}

	// Switch the collision component
	if (DetectComponent.IsValid())
	{
		DetectComponent->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	bAttackDetecting = bEnable;
}

void AWeaponModelBase::SetOwnerCharacter(ACharacter* InOwner)
{
	OwnerCharacter = InOwner;
}

void AWeaponModelBase::SetAttackDetectComponent(UPrimitiveComponent* InComponent)
{
	if (!IsValid(InComponent))
	{
		return;
	}

	// Unbind old component
	UnbindAttackDetectComponent();

	// Bind new one
	InComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeaponModelBase::OnAttackDetectingOverlapped);

	// Record new component
	DetectComponent = InComponent;
}

void AWeaponModelBase::UnbindAttackDetectComponent()
{
	if (DetectComponent.IsValid())
	{
		DetectComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModelBase::OnAttackDetectingOverlapped);
	}
}

void AWeaponModelBase::OnAttackDetectingOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Skip owner itself
	if (OtherActor == OwnerCharacter)
	{
		return;
	}

	// Trigger on hit function
	OnHitTarget(OtherActor, OtherComp, SweepResult);
}

void AWeaponModelBase::OnHitTarget(AActor* TargetActor, UPrimitiveComponent* TargetComponent,
	const FHitResult& HitResult)
{
	// Broadcast event
	HitEvent.Broadcast(TargetActor, TargetComponent, HitResult);
}
