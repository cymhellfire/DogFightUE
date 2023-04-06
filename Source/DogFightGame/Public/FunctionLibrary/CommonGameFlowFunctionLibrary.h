#pragma once

#include "CommonFunctionLibraryBase.h"
#include "CommonGameFlowFunctionLibrary.generated.h"

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
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static TArray<ATopDownStylePlayerController*> GetAllPlayerControllers(const UObject* WorldContextObject);

	/**
	 * Get the name of current processing game flow state.
	 *
	 * @return Name of current state.
	 */
	UFUNCTION(BlueprintCallable, Category="CommmonGameFlow")
	static FName GetCurrentGameFlowStateName(const UObject* WorldContextObject);

	/**
	 * Spawn character pawn for specified player controller.
	 *
	 * @param Controller The player control to spawn character.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SpawnPlayerCharacterPawn(ATopDownStylePlayerController* Controller);

	/**
	 * Switch character movement on/off for all players in current game.
	 *
	 * @param bEnable Whether to enable character movement.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SetCharacterMoveEnableForAllPlayers(const UObject* WorldContextObject, bool bEnable);

	/**
	 * Initialize timeline for current game.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void InitializeGameTimeline(const UObject* WorldContextObject);

	/**
	 * Get player id list of current timeline.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static TArray<int32> GetCurrentTimeline(const UObject* WorldContextObject);

	/**
	 * Move current timeline forward.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void MoveTimelineForward(const UObject* WorldContextObject);

	/**
	 * Get the ID of owner of current player round.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static int32 GetCurrentPlayerId(const UObject* WorldContextObject);

	/**
	 * Set the current player by ID.
	 * @param InId ID of new player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SetCurrentPlayerId(const UObject* WorldContextObject, int32 InId);

	/**
	 * Sync the CurrentPlayerId with first one of Timeline.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SyncCurrentPlayerIdWithTimeline(const UObject* WorldContextObject);

	/**
	 * Get the player controller of local player.
	 * @return			Player controller.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static ATopDownStylePlayerController* GetLocalPlayerController(const UObject* WorldContextObject);

	/**
	 * Get the local player.
	 * @return			First local player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static ULocalPlayer* GetLocalPlayer(const UObject* WorldContextObject);

	/**
	 * Get the player ID of local player.
	 * @return			ID of local player (-1 is illegal).
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static int32 GetLocalPlayerId(const UObject* WorldContextObject);

protected:
	/**
	 * Get current timeline which only works on server side.
	 */
	static UGameTimelineComponent* GetCurrentTimeline_Server(const UObject* WorldContextObject);

	/**
	 * Get current timeline which both works on server and client.
	 */
	static UGameTimelineComponent* GetCurrentTimeline_Common(const UObject* WorldContextObject);
};
