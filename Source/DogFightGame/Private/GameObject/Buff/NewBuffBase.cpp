// Dog Fight Game Code By CYM.


#include "GameObject/Buff/NewBuffBase.h"

#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"

UNewBuffBase::UNewBuffBase()
{
	ScriptPath = "DogFight.Buff.BuffBase";
}

void UNewBuffBase::InitBuff(const FString& InScriptPath)
{
	// Load script
	LoadBuffScript(InScriptPath);
}

void UNewBuffBase::Apply(ATopDownStylePlayerCharacter* InCharacter)
{
	if (!IsValid(InCharacter))
	{
		return;
	}

	AppliedCharacter = InCharacter;
	ApplyScript(InCharacter);
}

void UNewBuffBase::Remove()
{
	if (!IsValid(AppliedCharacter))
	{
		return;
	}

	RemoveScript(AppliedCharacter);
	AppliedCharacter = nullptr;
}
