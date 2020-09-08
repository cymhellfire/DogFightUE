// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"

#include "LobbyPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLobbyPlayerStateChangedSignature);

	UPROPERTY(BlueprintAssignable, Category="DogFight|Lobby")
	FLobbyPlayerStateChangedSignature OnLobbyPlayerStateChanged;

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void RemovePlayerState(APlayerState* PlayerState) override;

protected:

	UFUNCTION()
	void OnLobbyPlayerInfoChanged(int32 PlayerId, const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(Client, Reliable)
	void RpcLobbyPlayerInfoChanged();

public:

	UFUNCTION(BlueprintCallable, Category="DogFight|GameState")
	TArray<ALobbyPlayerState*> GetLobbyPlayerStates() const { return LobbyPlayerArray; }
	
protected:

	UPROPERTY(Replicated)
	TArray<ALobbyPlayerState*> LobbyPlayerArray;
};
