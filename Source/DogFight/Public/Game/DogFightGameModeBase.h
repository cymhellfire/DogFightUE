// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DogFightGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ADogFightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Finish current game and pump all players to main menu. */
	virtual void RequestFinishAndExitToMainMenu();
};
