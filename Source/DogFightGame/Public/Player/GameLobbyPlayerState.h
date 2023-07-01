// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameLobbyPlayerState.generated.h"

class AGameLobbyPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameLobbyPlayerBoolStatusChanged, AGameLobbyPlayerState*, PlayerState, bool, BoolStatus);

UCLASS()
class DOGFIGHTGAME_API AGameLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameLobbyPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server,Reliable)
	void ServerSetPlayerReadyStatus(bool bReady);

	UFUNCTION(Server,Reliable)
	void ServerSetPlayerHostStatus(bool bHost);

	UFUNCTION(BlueprintCallable, Category="GameLobbyPlayerState")
	bool GetPlayerReadyStatus() const
	{
		return bIsReady;
	}

	UFUNCTION(BlueprintCallable, Category="GameLobbyPlayerState")
	bool GetPlayerHostStatus() const
	{
		return bIsHost;
	}

protected:
	UFUNCTION()
	void OnRep_IsReady();

	UFUNCTION()
	void OnRep_IsHost();

public:
	UPROPERTY(BlueprintAssignable, Category="GameLobbyPlayerState")
	FGameLobbyPlayerBoolStatusChanged OnReadyStatusChanged;

	UPROPERTY(BlueprintAssignable, Category="GameLobbyPlayerState")
	FGameLobbyPlayerBoolStatusChanged OnHostStatusChanged;

protected:
	UPROPERTY(ReplicatedUsing=OnRep_IsReady)
	bool bIsReady;

	UPROPERTY(ReplicatedUsing=OnRep_IsHost)
	bool bIsHost;
};
