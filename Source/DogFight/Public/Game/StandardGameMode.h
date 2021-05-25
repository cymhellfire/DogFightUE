// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightGameModeBase.h"
#include "DogFightTypes.h"
#include "StandardGameMode.generated.h"

class AStandardModePlayerController;
class AStandardModeAIController;
class AStandardPlayerState;
class UGameplayCardPool;
class ADogFightAIController;

namespace GamePhase
{
	extern DOGFIGHT_API const FName EnteringMap;			// Players are entering this map, actors are not yet ticking
	extern DOGFIGHT_API const FName WaitingForStart;		// Actors are ticking, but the match has not yet begun
	extern DOGFIGHT_API const FName SpawnPlayers;			// Spawn character for players in game.
	extern DOGFIGHT_API const FName SpawnAIs;				// Spawn AI players for game.
	extern DOGFIGHT_API const FName FreeMoving;				// All players can move around without limitation.
	extern DOGFIGHT_API const FName DecideOrder;			// The order of players can take action will be decided here.
	extern DOGFIGHT_API const FName PlayerRoundBegin;		// Phase before a player's round begin.
	extern DOGFIGHT_API const FName PlayerRound;			// Specified player can take action in this phase.
	extern DOGFIGHT_API const FName CharacterReturn;		// Current player's character will return to the position where it's at round begin.
	extern DOGFIGHT_API const FName DiscardCards;			// Current player should discard all cards exceed the count limitation.
	extern DOGFIGHT_API const FName PlayerRoundEnd;			// Phase after a player's round end.
	extern DOGFIGHT_API const FName CheckGameEnd;			// Check if the game should end or continue.
	extern DOGFIGHT_API const FName GameSummary;			// Summary the game and display the result.
	extern DOGFIGHT_API const FName WaitingPostMatch;		// Match has ended but actors are still ticking
}

enum EGameModeDelayAction : uint8
{
	DA_PlayerCountCheck
};

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardGameMode : public ADogFightGameModeBase
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameModeDelegateWithPlayerId, int32, PlayerId);
	FGameModeDelegateWithPlayerId OnPrePlayerRoundBegin;
	FGameModeDelegateWithPlayerId OnPlayerRoundBegin;
	FGameModeDelegateWithPlayerId OnPlayerDiscardCard;
	FGameModeDelegateWithPlayerId OnPlayerRoundEnd;
	FGameModeDelegateWithPlayerId OnPlayerDead;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameModeDelegateNoArgument);
	FGameModeDelegateNoArgument OnPlayerResponseCardSelected;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameModeDelegateCameraEvent, FCameraFocusEvent, EventInfo);
	FGameModeDelegateCameraEvent OnCameraEventHappened;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StandardGameMode")
	TSubclassOf<UGameplayCardPool> CardPoolClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StandardGameMode")
	TSubclassOf<AStandardModeAIController> AIControllerClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode")
	int32 GameStartDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode", meta=(ClampMin="0.01", ClampMax="2"))
	float SpawnPlayerInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode")
	float FreeMovingDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode")
	int32 GameRoundInterval;

	TArray<EGameModeDelayAction> DelayActionQueue;

public:
	AStandardGameMode(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void EnablePlayerClickMovement();

	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void DisablePlayerClickMovement();

	virtual void Tick(float DeltaSeconds) override;

	/** Override version. Remove corresponding record in PlayerControllerList. */
	virtual void Logout(AController* Exiting) override;

	/** Override version. Send information to all connected clients. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void PreInitializeComponents() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual float CalculateDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FName GetGamePhase() const { return CurrentGamePhase; }

	/** Tell GameMode that the invoker is ready to start game. */
	virtual void PlayerReadyForGame(const FString& PlayerName);

	void RegisterPlayerToTimeline(AStandardModePlayerController* PlayerController);

	AStandardModePlayerController* GetPlayerControllerById(int32 PlayerId);

	AStandardModeAIController* GetAIControllerById(int32 PlayerId);

	/** Register a AIController to current game. */
	void RegisterAIController(AStandardModeAIController* NewController);

	void RegisterAIToTimeline(AStandardModeAIController* AIController);

	int32 GetAIControllerCount() const { return StandardAIControllerList.Num(); }

	/**
	 * Give random cards to specified player.
	 * @param PlayerId		Id of player to give cards.
	 * @param CardNum		Number of cards player will gain.
	 */
	void GivePlayerCards(int32 PlayerId, int32 CardNum);

	/**
	 * Give cards to specified player by giving a CardId.
	 * @param PlayerId		Id of player to give cards.
	 * @param CardNum		Number of cards player will gain.
	 * @param CardIndex		Index of card in CardPool.
	 */
	void GivePlayerCardsByCardIndex(int32 PlayerId, int32 CardNum, int32 CardIndex);

	virtual void StartGame();

	/** Send a localized message to specified player. */
	void SendGameMessageToPlayer(FGameMessage Message, int32 PlayerId);

	/** Broadcast a localized message to all players in current game. */
	void BroadcastGameMessageToAllPlayers(FGameMessage Message);

	/** Broadcast a localized title message to all players. */
	void BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage TitleMessage);

	/** End current player round. */
	void EndCurrentPlayerRound();

	int32 GetCurrentPlayerId() const { return CachedCurrentPlayerId; }

	/** Get a random AController from game. */
	AController* GetRandomController();

	bool IsAllPlayerNotRagdoll() const { return CurrentRagdollPlayerId.Num() == 0; };

	void AddPlayerInRagdoll(int32 PlayerId);

	void RemovePlayerInRagdoll(int32 PlayerId);

	class AStandardModePlayerCharacter* GetPlayerCharacterById(int32 PlayerId);

	int32 GetDefaultCardUsingCount() const { return DefaultCardUsingCount; }

	int32 ClampCardUsingCount(int32 InValue);

	int32 GetDefaultCardCapacity() const { return DefaultCardCapacity; }

	int32 ClampCardCapacity(int32 InValue);

	int32 GetDefaultCardGainPerRound() const { return DefaultCardGainPerRound; }

	int32 ClampCardGainPerRound(int32 InValue);

	/**
	 * Request a card with specified classes from target player.
	 * @param PlayerId				Id of target player.
	 * @param ResponseCardClasses	Array of card class requested.
	 * @param SourceActor			Actor that send request.
	 * @param CardToResponse		Localized card name to response.
	 */
	void RequestResponseCardFromPlayer(int32 PlayerId, TArray<TSubclassOf<class ACardBase>> ResponseCardClasses, AActor* SourceActor, const FText& CardToResponse);

	/**
	 * Transfer cards from source player to destination player.
	 * @param SrcPlayerState			Id of source player.
	 * @param DestPlayerState			Id of destination player.
	 * @param CardInfo				Detail information about cards to transfer.
	 * @return Actual card count that transferred.
	 */
	int32 TransferCardsBetweenPlayer(AStandardPlayerState* SrcPlayerState, AStandardPlayerState* DestPlayerState, struct FTransferCardInfo CardInfo);

	/**
	 * Broadcast a camera focus change event to all players.
	 * @param CameraEvent			Information of camera event.
	 */
	void BroadcastCameraFocusEvent(FCameraFocusEvent CameraEvent);
protected:
	virtual void BeginPlay() override;
	
	/** All PlayerController instances in current game. */
	TArray<AStandardModePlayerController*> StandardPlayerControllerList;

	/** All AIController instances in current game. */
	TArray<AStandardModeAIController*> StandardAIControllerList;

	FName CurrentGamePhase;

	FTimerHandle DefaultTimerHandle;
	FTimerHandle SecondaryTimerHandle;

	/** Number of players are already loaded this map. */
	uint32 PlayerJoinedGame;

	/** The index of player in list to spawn character. */
	int32 CurrentSpawnPlayerIndex;

	/** Indicate that current player is an AI. */
	bool bIsCurrentAIPlayer;

	/** Cached PlayerId for current round. */
	int32 CachedCurrentPlayerId;

	/** List of players are currently in Ragdoll state. */
	TArray<int32> CurrentRagdollPlayerId;

	/** Update remaining time. */
	virtual void DefaultTimer();

	virtual void SetGamePhase(FName NewPhase);

	virtual void OnGamePhaseChanged();

	virtual void HandlePhaseWaitingForStart();

	virtual void HandlePhaseGameSummary();

	virtual void HandlePhaseWaitingPostMatch();

	virtual void HandlePhaseSpawnPlayers();

	virtual void HandlePhaseSpawnAIs();

	virtual void HandlePhaseDecideOrder();

	UFUNCTION()
	void OnWeaponEquipped(AActor* CarrierActor);

	virtual void HandlePhasePlayerRoundBegin();

	UFUNCTION()
	virtual void OnPlayerBuffQueueBeginRoundFinished();

	virtual void HandlePhasePlayerRound();

	virtual void HandlePhaseCharacterReturn();

	UFUNCTION()
	virtual void OnCharacterReturnFinished(AActor* Actor);

	virtual void HandlePhaseDiscardCards();

	virtual void HandlePhasePlayerRoundEnd();

	UFUNCTION()
	virtual void OnPlayerBuffQueueEndRoundFinished();

	virtual void HandlePhaseCheckGameEnd();

	virtual void CheckGameEndAction();

	void SpawnPlayerTick();

	void SpawnAITick();

	virtual void HandlePhaseFreeMoving();

	virtual void OnJoinedPlayerCountChanged();

	UFUNCTION()
	void OnPlayerUsingCardFinished(bool bShouldEndRound);

	UFUNCTION()
	void OnPlayerDiscardCardFinished();

	UFUNCTION()
	void OnPlayerDeadCallback(int32 PlayerId);

	UFUNCTION()
	void OnAIPlayerDeadCallback(int32 PlayerId);

	UFUNCTION()
	void OnCharacterHealthChangedCallback(int32 PlayerId, int32 NewHealth);

	void HandleDelayAction(EGameModeDelayAction DelayAction);

	/** Add a DelayAction to the queue. */
	void PushDelayAction(EGameModeDelayAction DelayAction);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardGameMode")
	UGameplayCardPool* CardPool;

	void GivePlayerCards(AController* TargetController, AStandardPlayerState* TargetPlayerState, int32 CardNum);

	UFUNCTION()
	void OnResponseCardSelected(ACardBase* SelectedCard, AStandardPlayerState* ResponsePlayerState);

	void TransferCardBetweenPlayers_Internal(AStandardPlayerState* SrcPlayerState, AStandardPlayerState* DestPlayerState, int32 CardIndex);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	int32 DefaultCardUsingCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	FInt32Range CardUsingCountRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	int32 DefaultCardCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	FInt32Range CardCapacityRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	int32 DefaultCardGainPerRound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	FInt32Range CardGainPerRoundRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	TSubclassOf<class UWeaponBase> CharacterDefaultWeapon;

	int32 ReturnedCharacterCount;

	int32 WeaponEquipWaitingCharacterCount;

#pragma region Debug
public:
	UFUNCTION(Exec)
	void SetAllPlayerClickMove(bool bEnable);

	UFUNCTION(Exec)
	void GivePlayerCard(int32 PlayerId, int32 CardNum, int32 CardIndex);
#pragma endregion Debug
};
