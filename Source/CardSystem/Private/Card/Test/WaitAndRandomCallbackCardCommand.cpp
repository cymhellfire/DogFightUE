#include "Card/Test/WaitAndRandomCallbackCardCommand.h"

bool UWaitAndRandomCallbackCardCommand::Initialize(void* Param)
{
	if (!Super::Initialize(Param))
		return false;

	CAST_INIT_PARAM(Param, InitParam, FWaitAndRandomCallbackCardCommandParam);

	Time = InitParam->Time;
	Range = InitParam->RandomRange;
	return true;
}

void UWaitAndRandomCallbackCardCommand::Run()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UWaitAndRandomCallbackCardCommand::OnTimerExpired, Time);
}

void UWaitAndRandomCallbackCardCommand::OnTimerExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	int32 Result = FMath::CeilToInt32(FMath::RandRange(Range.GetMin(), Range.GetMax()));
	OnCommandFinished.Broadcast(this, Result);
}
