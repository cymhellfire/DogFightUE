// Dog Fight Game Code By CYM.


#include "Actors/Vfx/StealCardBeamVfx.h"

#include "Net/UnrealNetwork.h"

void AStealCardBeamVfx::SetStealCardData(int32 NewCardCount, FColor NewCardColor)
{
	CardItemColor = NewCardColor;
	CardCount = NewCardCount;
}

void AStealCardBeamVfx::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStealCardBeamVfx, CardCount);
	DOREPLIFETIME(AStealCardBeamVfx, CardItemColor);
}
