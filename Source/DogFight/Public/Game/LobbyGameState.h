// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "DogFightGameStateBase.h"

#include "LobbyPlayerState.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ALobbyGameState : public ADogFightGameStateBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLobbyPlayerStateChangedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameReadyChangedSignature, bool, IsGameReady);

	UPROPERTY(BlueprintAssignable, Category="DogFight|Lobby")
	FLobbyPlayerStateChangedSignature OnLobbyPlayerStateChanged;

	UPROPERTY(BlueprintAssignable, Category="DogFight|Lobby")
	FGameReadyChangedSignature OnGameReadyChanged;

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void RemovePlayerState(APlayerState* PlayerState) override;

protected:

	UFUNCTION()
	void OnLobbyPlayerInfoChanged();

public:

	UFUNCTION(BlueprintCallable, Category="DogFight|GameState")
	TArray<ALobbyPlayerState*> GetLobbyPlayerStates() const { return LobbyPlayerArray; }

	/** Are all players ready to play game. */
	UFUNCTION(BlueprintCallable, Category="DogFight|GameState")
	bool IsGameReady() const { return bIsGameReady;}


protected:
	/** All LobbyPlayerState in current game. */
	UPROPERTY()
	TArray<ALobbyPlayerState*> LobbyPlayerArray;

	/** Are all of players ready? */
	UPROPERTY(Replicated)
	bool bIsGameReady;

	bool IsGameReady();
};
