// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionAddChaos.h"
#include "Actors/Buffs/Buff_Chaos.h"

void UInstructionAddChaos::OnBuffCreated(IGameBuffInterface* BuffActor)
{
	if (ABuff_Chaos* ChaosBuff = Cast<ABuff_Chaos>(BuffActor))
	{
		ChaosBuff->ChaosChance = ChaosPossibility.GetValue();
	}
}
