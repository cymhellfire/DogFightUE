// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/DogFightGameStateBase.h"
#include "GameLobbyGameState.generated.h"

class AGameLobbyPlayerState;

UCLASS()
class DOGFIGHTGAME_API AGameLobbyGameState : public ADogFightGameStateBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameLobbyGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	TArray<AGameLobbyPlayerState*> GetAllPlayerState() const
	{
		return LobbyPlayerStateList;
	}

	UFUNCTION(BlueprintCallable, Category="GameLobbyGameState")
	bool GetGameReadyStatus() const
	{
		return bIsGameReady;
	}

	UFUNCTION(Server, Reliable)
	void ServerSetAIPlayerCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category="GameLobbyGameState")
	int32 GetAIPlayerCount() const
	{
		return AIPlayerCount;
	}

protected:
	void UpdateGameReadyStatus();

	void SetGameReady(bool bReady);

	UFUNCTION()
	void OnPlayerReadyStatusChanged(AGameLobbyPlayerState* PlayerState, bool bReady);

	UFUNCTION()
	void OnRep_IsGameReady();

	UFUNCTION()
	void OnRep_AIPlayerCount();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_IsGameReady)
	bool bIsGameReady;

	UPROPERTY(ReplicatedUsing=OnRep_AIPlayerCount)
	int32 AIPlayerCount;

	TArray<AGameLobbyPlayerState*> LobbyPlayerStateList;
};
