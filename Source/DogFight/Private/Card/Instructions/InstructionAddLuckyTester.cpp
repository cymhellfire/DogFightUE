// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionAddLuckyTester.h"

#include "Actors/Buffs/Buff_LuckyTester.h"

void UInstructionAddLuckyTester::OnBuffCreated(IGameBuffInterface* BuffActor)
{
	if (ABuff_LuckyTester* LuckyTesterBuff = Cast<ABuff_LuckyTester>(BuffActor))
	{
		LuckyTesterBuff->TesterDamage = Damage.GetValue();
		LuckyTesterBuff->TesterDamageRadius = DamageRadius.GetValue();
		LuckyTesterBuff->FailureRatio = TestFailRadio.GetValue();
	}
}
