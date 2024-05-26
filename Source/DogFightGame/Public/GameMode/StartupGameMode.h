// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "StartupGameMode.generated.h"

/**
 * This game mode is only used for game initialization.
 */
UCLASS()
class DOGFIGHTGAME_API AStartupGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
