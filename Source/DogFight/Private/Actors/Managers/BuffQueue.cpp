// Dog Fight Game Code By CYM.


#include "Actors/Managers/BuffQueue.h"
#include "Actors/Buffs/BuffBase.h"

void UBuffQueue::RegisterBuff(ABuffBase* NewBuff)
{
	if (!AttachedBuffList.Contains(NewBuff))
	{
		AttachedBuffList.Add(NewBuff);

		++BuffCountMap.FindOrAdd(NewBuff->GetClass()->GetFName());
	}
}

void UBuffQueue::UnregisterBuff(ABuffBase* TargetBuff)
{
	if (AttachedBuffList.Contains(TargetBuff))
	{
		AttachedBuffList.Remove(TargetBuff);

		const FName BuffName = TargetBuff->GetClass()->GetFName();
		if (BuffCountMap.Contains(BuffName))
		{
			BuffCountMap[BuffName]--;

			checkf(BuffCountMap[BuffName] >= 0, TEXT("Invalid buff count %d (class %s)"), BuffCountMap[BuffName], *BuffName.ToString());
		}
	}
}

void UBuffQueue::StartBuffCheckProcess()
{
	CurrentBuffIndex = AttachedBuffList.Num() - 1;
	ProcessCurrentBuff();
}

int32 UBuffQueue::GetBuffCountOfType(FName BuffClassName) const
{
	if (BuffCountMap.Contains(BuffClassName))
	{
		return BuffCountMap[BuffClassName];
	}

	return 0;
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
