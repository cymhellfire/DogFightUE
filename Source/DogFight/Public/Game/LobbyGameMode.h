// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "DogFightGameModeBase.h"
#include "LobbyPlayerState.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ALobbyGameMode : public ADogFightGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode(const FObjectInitializer& ObjectInitializer);

	/** Get all player info in current game. */
	TArray<FLobbyPlayerInfo> GetAllPlayerInfo() const;

protected:

	/** List of all player controller in current lobby. */
	TArray<class ALobbyPlayerController*> PlayerControllerList;
};
