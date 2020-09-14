// Dog Fight Game Code By CYM.


#include "StandardGameState.h"

AStandardGameState::AStandardGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MiniMapCamera = nullptr;
	RemainingTime = 0;
}

void AStandardGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardGameState, RemainingTime);
}

