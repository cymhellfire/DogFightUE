#pragma once

#include "GameMode/GameFlowBasedGameMode.h"
#include "FreeForAllGameMode.generated.h"

class AFreeForAllPlayerController;

UCLASS()
class DOGFIGHTGAME_API AFreeForAllGameMode : public AGameFlowBasedGameMode
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	void PlayerReadyForGame(AFreeForAllPlayerController* InPC);

protected:
	int32 ReadyPlayerCount;
};
