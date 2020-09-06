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

protected:

	UFUNCTION()
	virtual void OnLobbyMemberInfoChanged(int32 PlayerId, const FLobbyPlayerInfo& PlayerInfo);

	/** List of all player controller in current lobby. */
	TArray<class ALobbyPlayerController*> PlayerControllerList;
};
