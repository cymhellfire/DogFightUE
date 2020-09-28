// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightGameModeBase.h"
#include "DogFightTypes.h"
#include "StandardGameMode.generated.h"

class AStandardModePlayerController;

namespace GamePhase
{
	extern DOGFIGHT_API const FName EnteringMap;			// Players are entering this map, actors are not yet ticking
	extern DOGFIGHT_API const FName WaitingForStart;		// Actors are ticking, but the match has not yet begun
	extern DOGFIGHT_API const FName SpawnPlayers;			// Spawn character for players in game.
	extern DOGFIGHT_API const FName FreeMoving;				// All players can move around without limitation.
	extern DOGFIGHT_API const FName DecideOrder;			// The order of players can take action will be decided here.
	extern DOGFIGHT_API const FName PlayerRoundBegin;		// Phase before a player's round begin.
	extern DOGFIGHT_API const FName PlayerRound;			// Specified player can take action in this phase.
	extern DOGFIGHT_API const FName PlayerRoundEnd;			// Phase after a player's round end.
	extern DOGFIGHT_API const FName InProgress;				// Normal gameplay is occurring.
	extern DOGFIGHT_API const FName WaitingPostMatch;		// Match has ended but actors are still ticking
}

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardGameMode : public ADogFightGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode")
	int32 GameStartDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode", meta=(ClampMin="0.01", ClampMax="2"))
	float SpawnPlayerInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode")
	float FreeMovingDuration;

public:
	AStandardGameMode(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void EnablePlayerClickMovement();

	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void DisablePlayerClickMovement();

	/** Override version. Remove corresponding record in PlayerControllerList. */
	virtual void Logout(AController* Exiting) override;

	/** Override version. Send information to all connected clients. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void PreInitializeComponents() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	FName GetGamePhase() const { return CurrentGamePhase; }

	/** Tell GameMode that the invoker is ready to start game. */
	virtual void PlayerReadyForGame(const FString& PlayerName);

	virtual void StartGame();

	/** Broadcast a localized message to all players in current game. */
	void BroadcastGameMessageToAllPlayers(FGameMessage Message);
protected:
	virtual void BeginPlay() override;
	
	/** All PlayerController instances in current game. */
	TArray<AStandardModePlayerController*> StandardPlayerControllerList;

	FName CurrentGamePhase;

	FTimerHandle DefaultTimerHandle;
	FTimerHandle SecondaryTimerHandle;

	/** Number of players are already loaded this map. */
	uint32 PlayerJoinedGame;

	/** The index of player in list to spawn character. */
	int32 CurrentSpawnPlayerIndex;

	/** Update remaining time. */
	virtual void DefaultTimer();

	virtual void SetGamePhase(FName NewPhase);

	virtual void OnGamePhaseChanged();

	virtual void HandlePhaseWaitingForStart();

	virtual void HandlePhaseInProgress();

	virtual void HandlePhaseWaitingPostMatch();

	virtual void HandlePhaseSpawnPlayers();

	void SpawnPlayerTick();

	virtual void HandlePhaseFreeMoving();

	virtual void OnJoinedPlayerCountChanged();

#pragma region Debug
public:
	UFUNCTION(Exec)
	void SetAllPlayerClickMove(bool bEnable);
#pragma endregion Debug
};
