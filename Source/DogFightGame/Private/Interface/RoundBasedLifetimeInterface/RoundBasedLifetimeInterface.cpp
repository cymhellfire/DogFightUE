// Dog Fight Game Code By CYM.


#include "Interface/RoundBasedLifetimeInterface/RoundBasedLifetimeInterface.h"

#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "GameMode/TopDownStyleGameState.h"
#include "GameMode/GameStateComponent/GameTimelineComponent.h"


IRoundBasedLifetimeInterface::IRoundBasedLifetimeInterface()
	: Lifetime(0)
{
}

void IRoundBasedLifetimeInterface::SetLifetime(int32 InValue, FTimelineRoundTimerFinishDelegate& InCallback)
{
	Lifetime = InValue;

	// Get the game mode and timeline
	if (auto GameMode = UCommonGameplayFunctionLibrary::GetCurrentTopDownStyleGameState(GetWorldContext()))
	{
		if (auto TimelineComponent = GameMode->GetGameTimelineComponent())
		{
			TimelineComponent->AddTimer(Lifetime, InCallback);
		}
	}
}
