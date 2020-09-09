// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"


#include "LobbyPlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	/** Get all player info in current game. */
	TArray<FLobbyPlayerInfo> GetAllPlayerInfo() const;

protected:

	/** List of all player controller in current lobby. */
	TArray<class ALobbyPlayerController*> PlayerControllerList;
};
