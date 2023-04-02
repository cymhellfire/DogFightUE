// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/DogFightGameModeBase.h"
#include "DogFightTypes.h"
#include "Card/GameCardTypes.h"
#include "DamageCalculator/DamageCalculatorBase.h"
#include "DebugTools/ImGuiCommon.h"
#include "DebugTools/StandardGameMode_DT.h"
#include "StandardGameMode.generated.h"

class AStandardModePlayerController;
class AStandardModeAIController;
class AStandardPlayerState;
class UGameplayCardPool;
class UGameplayAbilityPool;
class ADogFightAIController;
class UDogFightDamageCalculatorBase;
class UGameFlowStateMachine;

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
	FGameModeDelegateNoArgument OnPlayerReadyForGame;
	FGameModeDelegateNoArgument OnRequestEndCurrentPlayerRound;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameModeDelegateCameraEvent, FCameraFocusEvent, EventInfo);
	FGameModeDelegateCameraEvent OnCameraEventHappened;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StandardGameMode")
	TSubclassOf<UGameplayCardPool> CardPoolClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StandardGameMode")
	TSubclassOf<UGameplayAbilityPool> AbilityPoolClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StandardGameMode")
	TSubclassOf<AStandardModeAIController> AIControllerClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode")
	int32 GameStartDelay;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode", meta=(ClampMin="0.01", ClampMax="2"))
	float SpawnPlayerInterval;

protected:
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

	virtual float CalculateDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FName GetGamePhase() const;

	/** Tell GameMode that the invoker is ready to start game. */
	virtual void PlayerReadyForGame(const FString& PlayerName);

	void RegisterPlayerToTimeline(AStandardModePlayerController* PlayerController);

	void RevivePlayer(int32 PlayerId);

	AStandardModePlayerController* GetPlayerControllerById(int32 PlayerId);

	TArray<AStandardModePlayerController*> GetAllPlayerControllers() const { return StandardPlayerControllerList; }

	AStandardModeAIController* GetAIControllerById(int32 PlayerId);

	TArray<AStandardModeAIController*> GetAllAIControllers() const { return StandardAIControllerList; }

	/** Register a AIController to current game. */
	void RegisterAIController(AStandardModeAIController* NewController);

	void RegisterAIToTimeline(AStandardModeAIController* AIController);

	int32 GetAIControllerCount() const { return StandardAIControllerList.Num(); }

	void SetCurrentAIPlayer(bool NewValue) { bIsCurrentAIPlayer = NewValue; }

	bool GetIsCurrentAIPlayer() const { return bIsCurrentAIPlayer; }

	bool IsHumanPlayer(int32 PlayerId) const
	{
		return HumanPlayerIdList.Contains(PlayerId);
	}

	bool IsAIPlayer(int32 PlayerId) const
	{
		return !IsHumanPlayer(PlayerId);
	}

	UGameplayAbilityPool* GetAbilityPool() const { return AbilityPool; }

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

	/** Send a localized message to specified player. */
	void SendGameMessageToPlayer(FGameMessage Message, int32 PlayerId);

	/** Broadcast a localized message to all players in current game. */
	void BroadcastGameMessageToAllPlayers(FGameMessage Message);

	/** Broadcast a localized title message to all players. */
	void BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage TitleMessage);

	/** End current player round. */
	void EndCurrentPlayerRound();

	int32 GetCurrentPlayerId() const { return CachedCurrentPlayerId; }

	void SetCurrentPlayerId(int32 NewId);

	/** Get a random AController from game. */
	AController* GetRandomController();

	bool IsAllPlayerNotRagdoll() const { return CurrentRagdollPlayerId.Num() == 0; };

	void AddPlayerInRagdoll(int32 PlayerId);

	void RemovePlayerInRagdoll(int32 PlayerId);

	AController* GetControllerById(int32 PlayerId);

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

	FVector GetCenterPointOfAllAlivePlayers() const;

	virtual void BeginDestroy() override;

	virtual void PostInitializeComponents() override;

	virtual AShieldManager* GetShieldManager() const { return ShieldManager; }

	virtual UGameModeStateMachine* GetGameModeStateMachine() { return GameModeStateMachine; }
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializeStateMachine();

	/** All PlayerController instances in current game. */
	TArray<AStandardModePlayerController*> StandardPlayerControllerList;

	/** All AIController instances in current game. */
	TArray<AStandardModeAIController*> StandardAIControllerList;

	/** Indicate that current player is an AI. */
	bool bIsCurrentAIPlayer;

	/** Cached PlayerId for current round. */
	int32 CachedCurrentPlayerId;

	/** List of players are currently in Ragdoll state. */
	TArray<int32> CurrentRagdollPlayerId;

	UFUNCTION()
	void OnGamePhaseChanged(FName NewPhase, uint8 SwitchMethod);

	virtual void OnJoinedPlayerCountChanged();

	UFUNCTION()
	void OnPlayerDeadCallback(int32 PlayerId);

	UFUNCTION()
	void OnAIPlayerDeadCallback(int32 PlayerId);

	UFUNCTION()
	void OnCharacterHealthChangedCallback(int32 PlayerId, int32 NewHealth);

	UFUNCTION()
	void OnCharacterStrengthChangedCallback(int32 PlayerId, int32 NewStrength);

	void HandleDelayAction(EGameModeDelayAction DelayAction);

	/** Add a DelayAction to the queue. */
	void PushDelayAction(EGameModeDelayAction DelayAction);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardGameMode")
	UGameplayCardPool* CardPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StandardGameMode")
	UGameplayAbilityPool* AbilityPool;

	void GivePlayerCards(AController* TargetController, AStandardPlayerState* TargetPlayerState, int32 CardNum);

	void TransferCardBetweenPlayers_Internal(AStandardPlayerState* SrcPlayerState, AStandardPlayerState* DestPlayerState, int32 CardIndex);

public:
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerSettings")
	int32 InitialAbilityCandidateCount;

	/** Class to calculate the damage to apply during game. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DogFightGameMode")
	TSubclassOf<UDogFightDamageCalculatorBase> DamageCalculatorClass;

protected:

	TArray<int32> HumanPlayerIdList;

	UPROPERTY()
	UDogFightDamageCalculatorBase* DamageCalculator;

	UPROPERTY()
	UDamageCalculatorBase* NewDamageCalculator;

	UPROPERTY()
	AShieldManager* ShieldManager;

	UPROPERTY(Transient)
	UGameModeStateMachine* GameModeStateMachine;

	UPROPERTY(Transient)
	UGameFlowStateMachine* GameFlowStateMachine;

#pragma region Debug
public:
	UFUNCTION(Exec)
	void SetAllPlayerClickMove(bool bEnable);

	UFUNCTION(Exec)
	void GivePlayerCard(int32 PlayerId, int32 CardNum, int32 CardIndex);

	UFUNCTION(Exec)
	void ToggleGameModeAdmin();

	UFUNCTION()
	void OnPlayerCardCountChanged(AStandardPlayerState* PlayerState);

	UFUNCTION()
	void OnPlayerRelationInfoChanged();

	UFUNCTION()
	void OnPlayerCardTargetAcquired(FCardInstructionTargetInfo& TargetInfo, int32 PlayerId);

#if WITH_IMGUI
	void ImGuiTick();

	FDebugGamePhaseHistoryRecord& GetCurrentGamePhaseRecord() { return StateMachineGamePhaseHistory.Last(); }

protected:
	void SetupDebugTools();
	void RemoveDebugTools();

	void GatherRelationshipInfo(int32 PlayerId);
	void GatherCardDisplayInfo(int32 PlayerId);
	FString GetPlayerNameById(int32 PlayerId);

	// Main window tabs
	void DrawPlayerBaseInfoTab();
	void DrawPlayerRelationInfoTab();
	void DrawCardManageTab();

	// State machine debugger
	void DrawStateMachineDebugger(bool* bOpen);

protected:
	uint8 bShowDebugTools : 1;
	uint8 bWasLastGamePhaseSelected : 1;
	bool bShowStateMachineDebugger;

	int32 MainWindowTabIndex;
	int32 PlayerIdShowBaseInfo;
	int32 PlayerIdShowRelationship;
	int32 PlayerIdManageCards;
	int32 CardIndexShowDetails;
	int32 GamePhaseHistorySelectIndex;
	int32 CachedGamePhaseHistoryCount;

	FDateTime GameStartTime;

	TMap<int32, FDebugPlayerBaseInfo> PlayerBaseInfoMap;
	TArray<FDebugPlayerRelationInfo> ShowingPlayerRelationships;
	TArray<FDebugCardDisplayInfo> SelectedPlayerCardInfoList;
	TArray<FDebugGamePhaseHistoryRecord> StateMachineGamePhaseHistory;
	FImGuiDelegateHandle ImGuiTickHandle;
#endif
#pragma endregion Debug
};
