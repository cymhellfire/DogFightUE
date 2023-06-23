// Dog Fight Game Code By CYM.


#include "GameObject/GameEffect/GameEffectStaticMesh.h"

#include "GameFramework/Character.h"


// Sets default values
AGameEffectStaticMesh::AGameEffectStaticMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create default components
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	MeshComponent->SetupAttachment(RootComponent);
}

void AGameEffectStaticMesh::SetTarget(AActor* InTarget)
{
	Super::SetTarget(InTarget);

	if (!IsValid(InTarget))
	{
		return;
	}

	if (AttachSocketName.IsNone())
	{
		SetActorLocation(InTarget->GetActorLocation());
	}
	else
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld, false);

		if (auto Character = Cast<ACharacter>(InTarget))
		{
			AttachToComponent(Character->GetMesh(), AttachRules, AttachSocketName);
		}
		else
		{
			AttachToActor(InTarget, AttachRules, AttachSocketName);
		}
	}
}
