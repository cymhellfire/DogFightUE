// Dog Fight Game Code By CYM.


#include "Ability/PermanentPassiveAbility.h"


UPermanentPassiveAbility::UPermanentPassiveAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCastType = EAbilityCastType::ACT_Passive;
}

void UPermanentPassiveAbility::RegisterAbility(AStandardPlayerState* OwnerPlayerState)
{
	Super::RegisterAbility(OwnerPlayerState);

	// Permanent passive ability should be activated once registered
	Activate();
}

void UPermanentPassiveAbility::UnregisterAbility()
{
	// Deactivate before unregister
	Deactivate();

	Super::UnregisterAbility();
}

bool UPermanentPassiveAbility::Activate()
{
	return true;
}

void UPermanentPassiveAbility::Deactivate()
{
	
}
