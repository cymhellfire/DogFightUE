// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectMacros.h"

#include "LobbyPlayerState.generated.h"

/**
 * Player status enum used in game lobby.
 */
UENUM(BlueprintType)
enum class EPlayerLobbyStatus : uint8
{
	Preparing,
	Ready,
	Host,
};

USTRUCT(BlueprintType)
struct FLobbyPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerId;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly)
	EPlayerLobbyStatus PlayerStatus;
};

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLobbyPlayerStateChangedSignature, int32, ChangedPlayerId, const FLobbyPlayerInfo&, PlayerInfo);

	/** Triggered when player state has changed. */
	FLobbyPlayerStateChangedSignature OnLobbyPlayerStateChanged;

	/** Update player state with provided player information. */
	UFUNCTION(BlueprintCallable, Category="DogFight|PlayerState")
	void SetPlayerInfo(UPARAM(ref) FLobbyPlayerInfo& PlayerState);

	/** Get current lobby status. */
	UFUNCTION(BlueprintCallable, Category="DogFight|PlayerState")
	EPlayerLobbyStatus GetLobbyStatus() const { return PlayerLobbyStatus; }

protected:
	/** Current player status in lobby. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=GameLobby)
	EPlayerLobbyStatus PlayerLobbyStatus;
};
