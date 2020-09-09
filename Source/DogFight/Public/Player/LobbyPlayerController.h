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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLobbyPlayerInfoChangeSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyGameReadyStateChangeSignature, bool, NewReadyState);

	UPROPERTY(BlueprintAssignable, Category="DogFight|Lobby")
	FLobbyPlayerInfoChangeSignature OnLobbyPlayerInfoChanged;

	UPROPERTY(BlueprintAssignable, Category="DogFight|Lobby")
	FLobbyGameReadyStateChangeSignature OnGameReadyStateChanged;

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

	/**
	 * Called by server when one player changed state.
	 */
	UFUNCTION(Client, Reliable)
	void RpcPlayerStateChanged();

	/**
	 * Called by server when game ready state changed.
	 */
	UFUNCTION(Client, Reliable)
	void RpcGameReadyStateChanged(bool bIsReady);

	virtual void OnRep_PlayerState() override;
protected:
	UFUNCTION(Server, Reliable)
	void CmdSendPlayerInfo(FLobbyPlayerInfo PlayerInfo);

private:
	void GatherPlayerInfo();
};
