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

void UBuffQueue::StopCurrentProcessingBuff()
{
	if (CurrentProcessingBuff != nullptr)
	{
		CurrentProcessingBuff->OnBuffProcessEndedEvent.RemoveDynamic(this, &UBuffQueue::OnBuffProcessEnded);
		CurrentProcessingBuff->OnBuffEndedEvent.RemoveDynamic(this, &UBuffQueue::OnBuffEnded);

		if (OnBuffQueueProcessFinished.IsBound())
		{
			OnBuffQueueProcessFinished.Clear();
		}
		CurrentProcessingBuff = nullptr;
	}
}

void UBuffQueue::ClearQueue()
{
	if (AttachedBuffList.Num() > 0)
	{
		for(int i = AttachedBuffList.Num() - 1; i >= 0; --i)
		{
			AttachedBuffList[i]->EndBuff();
		}

		AttachedBuffList.Empty();
	}

	BuffCountMap.Empty();
}

void UBuffQueue::ProcessCurrentBuff()
{
	CurrentProcessingBuff = AttachedBuffList[CurrentBuffIndex];
	if (CurrentProcessingBuff->IsBuffEnding())
	{
		CurrentProcessingBuff->OnBuffEndedEvent.AddDynamic(this, &UBuffQueue::OnBuffEnded);
		CurrentProcessingBuff->EndBuff();
	}
	else
	{
		CurrentProcessingBuff->OnBuffProcessEndedEvent.AddDynamic(this, &UBuffQueue::OnBuffProcessEnded);
		if (CurrentProcessPhase == EQP_RoundBegin)
		{
			CurrentProcessingBuff->OnTargetPlayerRoundBegin();
		}
		else if (CurrentProcessPhase == EQP_RoundEnd)
		{
			CurrentProcessingBuff->OnTargetPlayerRoundEnd();
		}
	}
}

void UBuffQueue::OnBuffEnded(ABuffBase* Buff)
{
	Buff->OnBuffEndedEvent.RemoveDynamic(this, &UBuffQueue::OnBuffEnded);
	CurrentProcessingBuff = nullptr;

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
	CurrentProcessingBuff = nullptr;

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
