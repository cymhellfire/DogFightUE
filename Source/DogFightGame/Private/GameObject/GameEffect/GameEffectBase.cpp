#include "GameObject/GameEffect/GameEffectBase.h"

AGameEffectBase::AGameEffectBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup root component
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	// Enable replication
	bReplicates = true;
}

void AGameEffectBase::PlayEffect_Implementation()
{
}
