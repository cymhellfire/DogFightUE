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
	FLobbyPlayerStateChangedSignature OnAIPlayerCountChanged;

	UPROPERTY(BlueprintAssignable, Category="DogFight|Lobby")
	FGameReadyChangedSignature OnGameReadyChanged;

	ALobbyGameState(const FObjectInitializer& ObjectInitializer);

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void RemovePlayerState(APlayerState* PlayerState) override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnLobbyPlayerInfoChanged();

	UFUNCTION()
	void OnRep_AIPlayerCount();

public:

	UFUNCTION(BlueprintCallable, Category="DogFight|GameState")
	TArray<ALobbyPlayerState*> GetLobbyPlayerStates() const { return LobbyPlayerArray; }

	/** Are all players ready to play game. */
	UFUNCTION(BlueprintCallable, Category="DogFight|GameState")
	bool IsGameReady() const { return bIsGameReady;}

	UFUNCTION(BlueprintCallable, Category="DogFight|GameState")
	void SetAIPlayerCount(int32 NewCount);

	UFUNCTION(BlueprintCallable, Category="DogFight|GameState")
	int32 GetAIPlayerCount() const { return AIPlayerCount; }
protected:
	/** All LobbyPlayerState in current game. */
	UPROPERTY()
	TArray<ALobbyPlayerState*> LobbyPlayerArray;

	/** Are all of players ready? */
	UPROPERTY(Replicated)
	bool bIsGameReady;

	bool IsGameReady();

	UPROPERTY(ReplicatedUsing=OnRep_AIPlayerCount)
	int32 AIPlayerCount;
};
