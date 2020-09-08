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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLobbyPlayerInfoChangeSignature, int32, ChangePlayerId, const TArray<FLobbyPlayerInfo>&, PlayerInfoArray);

	UPROPERTY(BlueprintAssignable, Category="DogFight|Lobby")
	FLobbyPlayerInfoChangeSignature OnLobbyPlayerInfoChanged;

public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);

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
