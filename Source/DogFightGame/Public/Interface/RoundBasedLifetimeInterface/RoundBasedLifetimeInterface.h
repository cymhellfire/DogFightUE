// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameMode/DataStruct/TimelineRoundTimer.h"
#include "RoundBasedLifetimeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URoundBasedLifetimeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that provide lifetime logic based on in-game rounds.
 */
class DOGFIGHTGAME_API IRoundBasedLifetimeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IRoundBasedLifetimeInterface();

	/**
	 * @brief Set the life span of this object.
	 * @param InValue			Life time in round unit.
	 * @param InCallback		Callback function when timer expired.
	 */
	virtual void SetLifetime(int32 InValue, FTimelineRoundTimerFinishDelegate& InCallback);

protected:
	virtual UObject* GetWorldContext() = 0;

protected:

	int32 Lifetime;
};
