// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameTimelineEntityInterface.generated.h"

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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetOwnerPlayerId() const = 0;

	virtual FString GetEntityName() const = 0;
};
