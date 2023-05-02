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

	// Invoke init
	OnBuffInitScript();
}

void UNewBuffBase::Finish()
{
	if (!IsValid(AppliedCharacter))
	{
		return;
	}

	// Let owner character starts removal progress
	AppliedCharacter->RemoveBuff(this);
	OnBuffFinishScript();
}

void UNewBuffBase::Apply(ATopDownStylePlayerCharacter* InCharacter)
{
	if (!IsValid(InCharacter))
	{
		return;
	}

	AppliedCharacter = InCharacter;
	ApplyScript(InCharacter);

	// Clear existing delay finish timer when applied to new target
	if (DelayFinishTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayFinishTimer);
	}
}

void UNewBuffBase::Remove()
{
	if (!IsValid(AppliedCharacter))
	{
		return;
	}

	RemoveScript(AppliedCharacter);
	AppliedCharacter = nullptr;

	// Set delay finish timer for next frame
	GetWorld()->GetTimerManager().SetTimer(DelayFinishTimer, this, &UNewBuffBase::DelayFinishExpired, 0.03f);
}

void UNewBuffBase::TransferTo(ATopDownStylePlayerCharacter* NewCharacter)
{
	if (!IsValid(AppliedCharacter))
	{
		return;
	}

	// Skip if target character is the same
	if (AppliedCharacter == NewCharacter)
	{
		return;
	}

	TransferToScript(NewCharacter);
	// Remove from current character and apply to new one
	AppliedCharacter->RemoveBuff(this);
	AppliedCharacter = NewCharacter;
	AppliedCharacter->AddBuff(this);
}

void UNewBuffBase::DelayFinishExpired()
{
	Finish();
}

void UNewBuffBase::DoBuffCheck(TEnumAsByte<EBuffCheckType::Type> InCheckType)
{
	// Invoke script function
	OnDoBuffCheckScript(InCheckType);
}

void UNewBuffBase::FinishDoCheck()
{
	// Invoke delegate
	OnDoCheckFinished.Broadcast();
}
