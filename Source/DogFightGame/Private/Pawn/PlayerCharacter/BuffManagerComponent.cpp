// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/BuffManagerComponent.h"
#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"
#include "GameObject/Buff/NewBuffBase.h"


// Sets default values for this component's properties
UBuffManagerComponent::UBuffManagerComponent()
	: BuffCheckInterval(1.f)
{
}

void UBuffManagerComponent::AddBuff(UNewBuffBase* InBuff)
{
	if (!IsValid(InBuff))
	{
		return;
	}

	// Do NOT add one buff multiple times
	if (AppliedBuffs.Contains(InBuff))
	{
		return;
	}

	// Apply new buff
	InBuff->Apply(GetTypedOuter<ATopDownStylePlayerCharacter>());
	AppliedBuffs.Add(InBuff);
}

void UBuffManagerComponent::RemoveBuff(UNewBuffBase* InBuff)
{
	if (!IsValid(InBuff))
	{
		return;
	}

	if (!AppliedBuffs.Contains(InBuff))
	{
		return;
	}

	// Remove buff from list
	InBuff->Remove();
	// Check if this buff is removed while buff checking
	if (CurCheckType.IsSet())
	{
		int32 Index;
		AppliedBuffs.Find(InBuff, Index);
		// Manually correct the checking index if previous buff is removed
		if (Index <= DoCheckIndex)
		{
			DoCheckIndex--;
		}
	}
	AppliedBuffs.Remove(InBuff);
}

void UBuffManagerComponent::DoBuffCheck(TEnumAsByte<EBuffCheckType::Type> InCheckType)
{
	DoCheckIndex = 0;
	CurCheckType = InCheckType;

	// Iterate through buff list and do check one by one
	StartDoBuffCheck();
}

void UBuffManagerComponent::StartDoBuffCheck()
{
	if (DoCheckIndex >= AppliedBuffs.Num())
	{
		OnAllBuffCheckFinished();
		return;
	}

	auto CheckBuff = AppliedBuffs[DoCheckIndex];
	DoCheckHandle = CheckBuff->OnDoCheckFinished.AddUObject(this, &UBuffManagerComponent::OnBuffCheckFinished);
	// Start check
	CheckBuff->DoBuffCheck(CurCheckType.GetValue());
}

void UBuffManagerComponent::OnBuffCheckFinished()
{
	auto CheckBuff = AppliedBuffs[DoCheckIndex];
	if (DoCheckHandle.IsValid())
	{
		CheckBuff->OnDoCheckFinished.Remove(DoCheckHandle);
	}

	// Increase the index and setup timer for next check
	DoCheckIndex++;
	GetWorld()->GetTimerManager().SetTimer(DoCheckIntervalHandle, this, &UBuffManagerComponent::OnDoCheckIntervalTimerExpired, BuffCheckInterval);
}

void UBuffManagerComponent::OnAllBuffCheckFinished()
{
	// Broadcast finish event
	OnDoCheckFinished.Broadcast();

	// Clear record
	CurCheckType.Reset();
}

void UBuffManagerComponent::OnDoCheckIntervalTimerExpired()
{
	StartDoBuffCheck();
}
