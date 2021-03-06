// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionWait.h"

UInstructionWait::UInstructionWait(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("Waiting");
	WaitDuration = 1.0f;
}

void UInstructionWait::Execute()
{
	Super::Execute();

	GetWorld()->GetTimerManager().SetTimer(WaitingTimerHandle, this, &UInstructionWait::OnWaitingFinished, WaitDuration, false);
}

void UInstructionWait::OnWaitingFinished()
{
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(WaitingTimerHandle);

	// This instruction is finished
	Finish();
}
