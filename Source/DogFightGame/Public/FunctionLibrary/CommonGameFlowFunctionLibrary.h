#pragma once

#include "CommonFunctionLibraryBase.h"
#include "GameMode/DataStruct/GameTimelineEntry.h"
#include "CommonGameFlowFunctionLibrary.generated.h"

class ATopDownStylePlayerCharacter;
class ATopDownStyleBotController;
class ATopDownStylePlayerController;
class UGameTimelineComponent;

UCLASS()
class DOGFIGHTGAME_API UCommonGameFlowFunctionLibrary : public UCommonFunctionLibraryBase
{
	GENERATED_BODY()
public:
	/**
	 * Get all player controller in current game.
	 *
	 * @return Array of all player controllers.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static TArray<ATopDownStylePlayerController*> GetAllPlayerControllers(UObject* WorldContextObject);

	/**
	 * Get all bot controllers in current game.
	 * 
	 * @return Array of all bot controllers;
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static TArray<ATopDownStyleBotController*> GetAllBotControllers(UObject* WorldContextObject);

	/**
	 * Get the name of current processing game flow state.
	 *
	 * @return Name of current state.
	 */
	UFUNCTION(BlueprintCallable, Category="CommmonGameFlow", meta=(WorldContext="WorldContextObject"))
	static FName GetCurrentGameFlowStateName(UObject* WorldContextObject);

	/**
	 * Spawn character pawn for specified player controller.
	 *
	 * @param Controller The player control to spawn character.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SpawnPlayerCharacterPawn(ATopDownStylePlayerController* Controller);

	/**
	 * Spawn character pawn for specified bot controller.
	 * 
	 * @param Controller The player controller to spawn character.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SpawnBotCharacterPawn(ATopDownStyleBotController* Controller);

	/**
	 * Switch character movement on/off for all players in current game.
	 *
	 * @param bEnable Whether to enable character movement.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void SetCharacterMoveEnableForAllPlayers(UObject* WorldContextObject, bool bEnable);

	/**
	 * Initialize timeline for current game.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void InitializeGameTimeline(UObject* WorldContextObject);

	/**
	 * Get player id list of current timeline.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static TArray<int32> GetCurrentTimeline(UObject* WorldContextObject);

	/**
	 * Move current timeline forward.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void MoveTimelineForward(UObject* WorldContextObject);

	/**
	 * Get the id of first player in timeline current order.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static int32 GetTimelineFirstPlayerId(UObject* WorldContextObject);

	/**
	 * Get the ID of owner of current player round.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static int32 GetCurrentPlayerId(UObject* WorldContextObject);

	/**
	 * Set the current player by ID.
	 * @param InId ID of new player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void SetCurrentPlayerId(UObject* WorldContextObject, int32 InId);

	/**
	 * Get game timeline component of current game state.
	 * @return Timeline component.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static UGameTimelineComponent* GetCurrentTimelineComponent(UObject* WorldContextObject);

	/**
	 * Sync the CurrentPlayerId with first one of Timeline.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void SyncCurrentPlayerIdWithTimeline(UObject* WorldContextObject);

	/**
	 * Get Id of current timeline entity.
	 * @return Id of current timeline entity.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static int32 GetCurrentTimelineEntityId(UObject* WorldContextObject);

	/**
	 * Get type of current timeline entity.
	 * @return Type of current timeline entity.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static EGameTimelineEntityType::Type GetCurrentTimelineEntityType(UObject* WorldContextObject);

	/**
	 * Get the character of current timeline entity.
	 * @return Character of current timeline entity.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static ATopDownStylePlayerCharacter* GetCurrentTimelineEntityCharacter(UObject* WorldContextObject);

	/**
	 * Send a request to finish local player's round immediately.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void RequestFinishLocalPlayerRound(UObject* WorldContextObject);

	/**
	 * Broadcast player round start event to all players.
	 * @param PlayerId Id of player that start round.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void BroadcastStartPlayerRound(UObject* WorldContextObject, int32 PlayerId);

	/**
	 * Broadcast player round finish event to all players.
	 * @param PlayerId Id of player that finish round.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static void BroadcastFinishPlayerRound(UObject* WorldContextObject, int32 PlayerId);

	/**
	 * Get the player controller of local player.
	 * @return			Player controller.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static ATopDownStylePlayerController* GetLocalPlayerController(UObject* WorldContextObject);

	/**
	 * Get the local player.
	 * @return			First local player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static ULocalPlayer* GetLocalPlayer(UObject* WorldContextObject);

	/**
	 * Get the player ID of local player.
	 * @return			ID of local player (-1 is illegal).
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static int32 GetLocalPlayerId(UObject* WorldContextObject);

	/**
	 * Spawn a new Bot player into current game.
	 * @return Id of new added player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow", meta=(WorldContext="WorldContextObject"))
	static int32 SpawnBotPlayer(UObject* WorldContextObject);

protected:
	/**
	 * Get current timeline which only works on server side.
	 */
	static UGameTimelineComponent* GetCurrentTimeline_Server(UObject* WorldContextObject);

	/**
	 * Get current timeline which both works on server and client.
	 */
	static UGameTimelineComponent* GetCurrentTimeline_Common(UObject* WorldContextObject);
};
