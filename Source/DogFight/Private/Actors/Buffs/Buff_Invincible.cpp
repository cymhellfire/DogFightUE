// Dog Fight Game Code By CYM.


#include "Buff_Invincible.h"

#include "ActorInterfaces.h"

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
