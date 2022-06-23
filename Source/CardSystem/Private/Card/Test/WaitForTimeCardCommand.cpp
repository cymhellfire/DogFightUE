#include "Card/Test/WaitForTimeCardCommand.h"

bool UWaitForTimeCardCommand::Initialize(void* Param)
{
	if (!Super::Initialize(Param))
		return false;

	FWaitForTimeCardCommandParam* InitParam = reinterpret_cast<FWaitForTimeCardCommandParam*>(Param);
	if (InitParam == nullptr)
	{
		return false;
	}

	WaitTime = InitParam->Time;
	return true;
}

void UWaitForTimeCardCommand::Run()
{
	if (WaitTime <= 0)
	{
		OnTimerFinished();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UWaitForTimeCardCommand::OnTimerFinished, WaitTime);
}

void UWaitForTimeCardCommand::OnTimerFinished()
{
	// Unregister timer callback
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	OnCommandFinished.Broadcast(this, true);
}
