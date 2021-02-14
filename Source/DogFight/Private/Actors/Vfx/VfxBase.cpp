// Dog Fight Game Code By CYM.


#include "Actors/Vfx/VfxBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AVfxBase::AVfxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");

	// Create the audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVfxBase::BeginPlay()
{
	Super::BeginPlay();

	// Set life span
	SetLifeSpan(Lifetime);

	// Auto play audio clip as default
	AudioComponent->Play();
}

void AVfxBase::SetTargetActor(AActor* Target)
{
	check(Target);

	// Check attach socket
	if (SocketName.IsNone())
	{
		SetActorLocation(Target->GetActorLocation());
	}
	else
	{
		const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
		if (ACharacter* Character = Cast<ACharacter>(Target))
		{
			AttachToComponent(Character->GetMesh(), AttachmentTransformRules, SocketName);
		}
		else
		{
			AttachToActor(Target, AttachmentTransformRules, SocketName);
		}
	}
}

