#pragma once

#include "CommonFunctionLibraryBase.h"
#include "Common/DamageDisplayParams.h"
#include "CommonGameplayFunctionLibrary.generated.h"

class UCard;
class ATopDownStylePlayerController;
class ATopDownStylePlayerState;
class AGameEffectBase;

UCLASS()
class DOGFIGHTGAME_API UCommonGameplayFunctionLibrary : public UCommonFunctionLibraryBase
{
	GENERATED_BODY()
public:
	/**
	 * Get PlayerState by player id.
	 * @param InPlayerId		Id of player that acquire state for.
	 * @return					The PlayerState of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static APlayerState* GetPlayerStateById(int32 InPlayerId);

	/**
	 * Get PlayerController by player id.
	 * @param InPlayerId		Id of player that acquire state for.
	 * @return					The PlayerController of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static ATopDownStylePlayerController* GetPlayerControllerById(int32 InPlayerId);

	/**
	 * Given player specified card.
	 * @param InPlayerId		Id of player that cards given to.
	 * @param InCard			The card that given to player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card")
	static void DispatchCardToPlayer(int32 InPlayerId, UCard* InCard);

	/**
	 * Let local player use card by instance id.
	 * @param InInstanceId		Instance Id of card that to be used.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card")
	static void UseCardByInstanceId(int32 InInstanceId);

	/**
	 * Get card count of specified player.
	 * @param InPlayerId		Player id to get card count.
	 * @return					Total card count of player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card")
	static int32 GetPlayerCardNums(int32 InPlayerId);

	/**
	 * Add specified game widget to player with given id.
	 * @param WidgetName		Name of widget to show.
	 * @param InPlayerId		Id of player to add widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void AddWidgetByPlayerId(FString WidgetName, int32 InPlayerId);

	/**
	 * Show specified game widget to player with given id.
	 * @param WidgetName		Name of widget to show.
	 * @param InPlayerId		Id of player to change visibility.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void ShowWidgetByPlayerId(FString WidgetName, int32 InPlayerId);

	/**
	 * Hide specified game widget to player with given id.
	 * @param WidgetName		Name of widget to hide.
	 * @param InPlayerId		Id of player to hide widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void HideWidgetByPlayerId(FString WidgetName, int32 InPlayerId);

	/**
	 * Remove specified game widget from player with given id.
	 * @param WidgetName		Name of widget to remove.
	 * @param InPlayerId		Id of player to remove widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void RemoveWidgetPlayerId(FString WidgetName, int32 InPlayerId);

	/**
	 * Create damage display item to player with given id.
	 * @param DisplayParams		Display parameter set.
	 * @param InPlayerId		Id of player to show.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void CreateDamageDisplayByPlayerId(const FDamageDisplayParams& DisplayParams, int32 InPlayerId);

	/**
	 * Get the total count of alive player in current game.
	 * @return			Number of players are still alive.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static int32 GetAlivePlayerNum();

	/**
	 * Spawn game effect with given id at specified location on every connected client.
	 * @param EffectId			Id of game effect to spawn.
	 * @param Pos				Spawn position.
	 * @param Rot				Spawn rotation.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Effect")
	static void SpawnGameEffectAtPos(int32 EffectId, FVector Pos, FRotator Rot);

	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void DamageActor(int32 DamageId, AActor* Target, float BaseDamage, AActor* Causer);
protected:
	/**
	 * Do specified function on every player state in current game meet the id mask.
	 */
	static void ForEachPlayerStateDo(TFunction<void(ATopDownStylePlayerState*)> ExecuteFunc, int32 PlayerIdMask = -1);

	/**
	 * Do specified function on every player controller in current game meet the id mask.
	 */
	static void ForEachPlayerControllerDo(TFunction<void(ATopDownStylePlayerController*)> ExecuteFunc, int32 PlayerIdMask = -1);
};
