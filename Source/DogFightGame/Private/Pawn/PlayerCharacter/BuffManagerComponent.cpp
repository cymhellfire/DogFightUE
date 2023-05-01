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

