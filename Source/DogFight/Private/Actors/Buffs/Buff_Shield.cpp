#include "Actors/Buffs/Buff_Shield.h"
#include "Actors/Shield/ShieldBase.h"

void ABuff_Shield::SetShieldActor(AShieldBase* NewShield)
{
	ShieldActor = NewShield;
}

void ABuff_Shield::RemoveBuff()
{
	if (IsValid(ShieldActor))
	{
		ShieldActor->Destroy();
	}

	Super::RemoveBuff();
}
