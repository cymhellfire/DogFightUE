// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameTimelineEntityInterface.generated.h"

class FGameTimelineEntry;

// This class does not need to be modified.
UINTERFACE()
class UGameTimelineEntityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOGFIGHTGAME_API IGameTimelineEntityInterface
{
	GENERATED_BODY()

	friend FGameTimelineEntry;
public:
	virtual int32 GetOwnerPlayerId() const = 0;

	virtual FString GetEntityName() const = 0;

	int32 GetEntityId() const
	{
		return EntityId;
	}

private:
	int32 EntityId;
};
