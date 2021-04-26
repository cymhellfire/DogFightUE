// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_Invincible.h"
#include "Actors/Components/ReceiveDamageComponent.h"
#include "Actors/Interfaces/DamageableActorInterface.h"

void ABuff_Invincible::ApplyBuff()
{
	Super::ApplyBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->AddInvincibleFlags(static_cast<int32>(EActorInvincibleFlags::AIF_GameplayBuff));
	}
}

void ABuff_Invincible::RemoveBuff()
{
	Super::RemoveBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->RemoveInvincibleFlags(static_cast<int32>(EActorInvincibleFlags::AIF_GameplayBuff));
	}
}
