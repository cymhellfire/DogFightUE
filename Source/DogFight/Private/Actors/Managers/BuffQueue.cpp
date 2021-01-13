// Dog Fight Game Code By CYM.


#include "Actors/Managers/BuffQueue.h"
#include "Actors/Buffs/BuffBase.h"

void UBuffQueue::RegisterBuff(ABuffBase* NewBuff)
{
	if (!AttachedBuffList.Contains(NewBuff))
	{
		AttachedBuffList.Add(NewBuff);
	}
}

void UBuffQueue::UnregisterBuff(ABuffBase* TargetBuff)
{
	if (AttachedBuffList.Contains(TargetBuff))
	{
		AttachedBuffList.Remove(TargetBuff);
	}
}

void UBuffQueue::StartBuffCheckProcess()
{
	CurrentBuffIndex = AttachedBuffList.Num() - 1;
	ProcessCurrentBuff();
}

void UBuffQueue::ProcessCurrentBuff()
{
	ABuffBase* CurrentBuff = AttachedBuffList[CurrentBuffIndex];
	if (CurrentBuff->IsBuffEnding())
	{
		CurrentBuff->OnBuffEndedEvent.AddDynamic(this, &UBuffQueue::OnBuffEnded);
		CurrentBuff->EndBuff();
	}
}

void UBuffQueue::OnBuffEnded(ABuffBase* Buff)
{
	Buff->OnBuffEndedEvent.RemoveDynamic(this, &UBuffQueue::OnBuffEnded);

	// Process to next buff
	CurrentBuffIndex--;
	if (CurrentBuffIndex >= 0)
	{
		ProcessCurrentBuff();
	}
	else
	{
		OnBuffQueueProcessFinished.Broadcast();
	}
}
