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

	/** Notify all clients that game will start. */
	virtual void NotifyClientGameWillStart();

protected:
	
	/** List of all player controller in current game. */
	TArray<class ADogFightPlayerController*> PlayerControllerList;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
};
