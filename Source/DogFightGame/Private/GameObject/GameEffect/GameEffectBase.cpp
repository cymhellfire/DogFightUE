#include "GameObject/GameEffect/GameEffectBase.h"

AGameEffectBase::AGameEffectBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup root component
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}
