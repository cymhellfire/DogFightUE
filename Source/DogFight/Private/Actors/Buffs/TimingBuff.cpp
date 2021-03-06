// Dog Fight Game Code By CYM.


#include "Actors/Buffs/TimingBuff.h"

ATimingBuff::ATimingBuff()
	: Super()
{
	TriggeredEndTimerDuration = 2.f;
}

void ATimingBuff::RemoveBuff()
{
	// Only take actions when this buff is end normally
	if (LifetimeQueue.Num() == 0)
	{
		TimerExpired();
	}

	Super::RemoveBuff();
}

void ATimingBuff::TimerExpired()
{
	// Spawn Vfx actor
	if (IsValid(ActionVfxClass))
	{
		AActor* NewVfxActor = GetWorld()->SpawnActor<AActor>(ActionVfxClass);
		NewVfxActor->SetActorLocation(TargetActor->GetActorLocation());
	}

	// Modify the end timer when successfully triggered
	BuffEndingDuration = TriggeredEndTimerDuration;
}
