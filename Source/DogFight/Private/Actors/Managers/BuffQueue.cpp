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

void UBuffQueue::StartRoundBeginBuffCheckProcess()
{
	CurrentBuffIndex = AttachedBuffList.Num() - 1;
	CurrentProcessPhase = EQP_RoundBegin;
	ProcessCurrentBuff();
}

void UBuffQueue::StartRoundEndBuffCheckProcess()
{
	CurrentBuffIndex = AttachedBuffList.Num() - 1;
	CurrentProcessPhase = EQP_RoundEnd;
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
	else
	{
		CurrentBuff->OnBuffProcessEndedEvent.AddDynamic(this, &UBuffQueue::OnBuffProcessEnded);
		if (CurrentProcessPhase == EQP_RoundBegin)
		{
			CurrentBuff->OnTargetPlayerRoundBegin();
		}
		else if (CurrentProcessPhase == EQP_RoundEnd)
		{
			CurrentBuff->OnTargetPlayerRoundEnd();
		}
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

void UBuffQueue::OnBuffProcessEnded(ABuffBase* Buff)
{
	Buff->OnBuffProcessEndedEvent.RemoveDynamic(this, &UBuffQueue::OnBuffProcessEnded);

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
