#include "GameMode/TopDownStyleGameState.h"

ATopDownStyleGameState::ATopDownStyleGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WorldBounds = FBox(FVector(-2000, -2000, 0.f), FVector(2000, 2000, 0.f));
}
