// Dog Fight Game Code By CYM.


#include "Ability/InstantAggressiveAbility.h"

UInstantAggressiveAbility::UInstantAggressiveAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCastType = EAbilityCastType::ACT_Aggressive;
}
