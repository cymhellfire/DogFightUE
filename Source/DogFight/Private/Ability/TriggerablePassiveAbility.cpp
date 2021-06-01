// Dog Fight Game Code By CYM.


#include "Ability/TriggerablePassiveAbility.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"

UTriggerablePassiveAbility::UTriggerablePassiveAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCastType = EAbilityCastType::ACT_Passive;
}

void UTriggerablePassiveAbility::RegisterAbility(AStandardPlayerState* OwnerPlayerState)
{
	Super::RegisterAbility(OwnerPlayerState);

	// Register trigger condition
	RegisterCondition();
}

void UTriggerablePassiveAbility::UnregisterAbility()
{
	// Unregister trigger condition
	UnregisterCondition();

	Super::UnregisterAbility();
}

void UTriggerablePassiveAbility::RegisterCondition()
{
	
}

void UTriggerablePassiveAbility::UnregisterCondition()
{
	
}
