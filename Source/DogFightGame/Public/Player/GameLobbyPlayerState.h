// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Subsystem/InGameStatistics/GameLobbyPlayerInfo.h"
#include "GameLobbyPlayerState.generated.h"

class AGameLobbyPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameLobbyPlayerBoolStatusChanged, AGameLobbyPlayerState*, PlayerState, bool, BoolStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameLobbyPlayerStringStatusChanged, AGameLobbyPlayerState*, PlayerState, const FString&, String);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameLobbyPlayerAvatarIdChanged, AGameLobbyPlayerState*, PlayerState, const FGameLobbyPlayerAvatarId&, AvatarId);

DECLARE_MULTICAST_DELEGATE_TwoParams(FGameLobbyPlayerInfoChangedEvent, AGameLobbyPlayerState*, const FGameLobbyPlayerInfo&);

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
	void ServerUpdatePlayerInfo(const FGameLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Category="GameLobbyPlayerState")
	bool GetPlayerReadyStatus() const
	{
		return bIsReady;
	}

	UFUNCTION(BlueprintCallable, Category="GameLobbyPlayerState")
	bool GetPlayerHostStatus() const
	{
		return LobbyPlayerInfo.bHost;
	}

	/**
	 * Get current lobby player info.
	 * @return Copy of current lobby player info.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyPlayerState")
	FGameLobbyPlayerInfo GetLobbyPlayerInfo() const
	{
		return LobbyPlayerInfo;
	}

	UFUNCTION()
	FString GetUniqueNetIdStr() const
	{
		auto UniqueNetId = GetUniqueId();
		return UniqueNetId.ToString();
	}

protected:
	UFUNCTION()
	void OnRep_IsReady();

	UFUNCTION()
	void OnRep_LobbyPlayerInfo(const FGameLobbyPlayerInfo& OldData);

	virtual void OnRep_PlayerName() override;

public:
	UPROPERTY(BlueprintAssignable, Category="GameLobbyPlayerState")
	FGameLobbyPlayerStringStatusChanged OnPlayerNameChanged;

	UPROPERTY(BlueprintAssignable, Category="GameLobbyPlayerState")
	FGameLobbyPlayerBoolStatusChanged OnReadyStatusChanged;

	UPROPERTY(BlueprintAssignable, Category="GameLobbyPlayerState")
	FGameLobbyPlayerBoolStatusChanged OnHostStatusChanged;

	UPROPERTY(BlueprintAssignable, Category="GameLobbyPlayerState")
	FGameLobbyPlayerAvatarIdChanged OnAvatarIdChanged;

	FGameLobbyPlayerInfoChangedEvent OnPlayerLobbyInfoChanged;

protected:
	UPROPERTY(ReplicatedUsing=OnRep_IsReady)
	bool bIsReady;

	UPROPERTY(ReplicatedUsing=OnRep_LobbyPlayerInfo)
	FGameLobbyPlayerInfo LobbyPlayerInfo;
};
