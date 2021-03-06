// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_IncredibleSurvive.h"

#include "Actors/Interfaces/DamageableActorInterface.h"

ABuff_IncredibleSurvive::ABuff_IncredibleSurvive()
	: Super()
{
	HealthPercentage = 0.1f;
	RecoverPercentage = 1.f;
}

void ABuff_IncredibleSurvive::ApplyBuff()
{
	Super::ApplyBuff();

	// Change target actor health
	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->SetHealthPercentage(HealthPercentage);
	}
}

void ABuff_IncredibleSurvive::TimerExpired()
{
	// Restore target actor health
	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		if (DamageableActor->IsAlive())
		{
			DamageableActor->SetHealthPercentage(RecoverPercentage);

			Super::TimerExpired();
		}
	}
}
