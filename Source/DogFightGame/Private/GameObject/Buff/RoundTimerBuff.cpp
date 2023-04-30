// Dog Fight Game Code By CYM.


#include "GameObject/Buff/RoundTimerBuff.h"

#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"

void URoundTimerBuff::SetDuration(int32 InTime)
{
	FTimelineRoundTimerFinishDelegate FinishDelegate;
	FinishDelegate.BindLambda([this]()
	{
		// Remove buff when expired
		if (IsValid(AppliedCharacter))
		{
			AppliedCharacter->RemoveBuff(this);
		}
		else
		{
			Remove();
		}
	});

	SetLifetime(InTime, FinishDelegate);
}
