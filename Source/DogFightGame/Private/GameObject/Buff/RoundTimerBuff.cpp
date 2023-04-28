// Dog Fight Game Code By CYM.


#include "GameObject/Buff/RoundTimerBuff.h"

void URoundTimerBuff::SetDuration(int32 InTime)
{
	FTimelineRoundTimerFinishDelegate FinishDelegate;
	FinishDelegate.BindLambda([this]()
	{
		Remove();
	});

	SetLifetime(InTime, FinishDelegate);
}
