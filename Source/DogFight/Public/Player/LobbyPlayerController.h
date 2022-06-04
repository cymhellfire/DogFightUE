// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Player/DogFightPlayerController.h"
#include "Game/LobbyPlayerState.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ALobbyPlayerController : public ADogFightPlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="DogFight|Lobby")
	void ToggleReadyStatus();

protected:
	UFUNCTION(Server, Reliable)
	void ServerSendPlayerInfo(FLobbyPlayerInfo PlayerInfo);

	virtual void GatherPlayerInfo() override;
};
