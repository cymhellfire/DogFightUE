// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "LobbyPlayerState.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="DogFight|Lobby")
	void ToggleReadyStatus();

	/**
	 * Gather all information and send to server (only for host).
	 * Client use OnRep_PlayerState to send player info.
	 */
	UFUNCTION(Client, Reliable)
	void RpcHostUploadPlayerInfo();

	virtual void OnRep_PlayerState() override;
protected:
	UFUNCTION(Server, Reliable)
	void CmdSendPlayerInfo(FLobbyPlayerInfo PlayerInfo);

private:
	void GatherPlayerInfo();
};
