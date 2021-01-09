// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionAddArmor.h"
#include "BasicArmorBuff.h"

void UInstructionAddArmor::OnBuffCreated(IGameBuffInterface* BuffActor)
{
	if (ABasicArmorBuff* ArmorBuffActor = Cast<ABasicArmorBuff>(BuffActor))
	{
		// Setup the armor setting
		ArmorBuffActor->InitializeArmor(ArmorValue.GetValue(), AntiDamageCategories, DesiredDamageTypes);
	}
}
