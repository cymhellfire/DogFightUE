// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/BuffManagerComponent.h"
#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"
#include "GameObject/Buff/NewBuffBase.h"


// Sets default values for this component's properties
UBuffManagerComponent::UBuffManagerComponent()
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
	CheckBuff->DoBuffCheck(CurCheckType);
}

void UBuffManagerComponent::OnBuffCheckFinished()
{
	auto CheckBuff = AppliedBuffs[DoCheckIndex];
	if (DoCheckHandle.IsValid())
	{
		CheckBuff->OnDoCheckFinished.Remove(DoCheckHandle);
	}

	// Increase the index and do next check
	DoCheckIndex++;
	StartDoBuffCheck();
}

void UBuffManagerComponent::OnAllBuffCheckFinished()
{
	// Broadcast finish event
	OnDoCheckFinished.Broadcast();
}
