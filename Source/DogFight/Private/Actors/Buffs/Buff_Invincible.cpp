// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_Invincible.h"

#include "Actors/Interfaces/DamageableActorInterface.h"

void ABuff_Invincible::ApplyBuff()
{
	Super::ApplyBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->SetInvincible(true);
	}
}

void ABuff_Invincible::RemoveBuff()
{
	Super::RemoveBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->SetInvincible(false);
	}
}
