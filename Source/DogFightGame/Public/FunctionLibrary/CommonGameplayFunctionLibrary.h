#pragma once

#include "CommonFunctionLibraryBase.h"
#include "Common/DamageDisplayParams.h"
#include "Common/WeaponCommon.h"
#include "CommonGameplayFunctionLibrary.generated.h"

class UCard;
class ATopDownStylePlayerController;
class ATopDownStylePlayerCharacter;
class ATopDownStylePlayerState;
class AGameEffectBase;
class UGameplayDataSubsystem;

UCLASS()
class DOGFIGHTGAME_API UCommonGameplayFunctionLibrary : public UCommonFunctionLibraryBase
{
	GENERATED_BODY()
	friend class UCommonGameFlowFunctionLibrary;
public:
	/**
	 * Get gameplay data subsystem.
	 * @return Gameplay data subsystem of current game.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static UGameplayDataSubsystem* GetGameplayDataSubsystem(const UObject* WorldContextObject);

	/**
	 * Get PlayerState by player id.
	 * @param InPlayerId		Id of player that acquire state for.
	 * @return					The PlayerState of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static APlayerState* GetPlayerStateById(const UObject* WorldContextObject, int32 InPlayerId);

	/**
	 * Get PlayerController by player id.
	 * @param InPlayerId		Id of player that acquire controller for.
	 * @return					The PlayerController of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static ATopDownStylePlayerController* GetPlayerControllerById(const UObject* WorldContextObject, int32 InPlayerId);

	/**
	 * Get character pawn of specified player.
	 * @param InPlayerId		Id of player that acquire character for.
	 * @return					The player character of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static ATopDownStylePlayerCharacter* GetPlayerCharacterById(const UObject* WorldContextObject, int32 InPlayerId);

	/**
	 * Given player specified card.
	 * @param InPlayerId		Id of player that cards given to.
	 * @param InCard			The card that given to player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card")
	static void DispatchCardToPlayer(const UObject* WorldContextObject, int32 InPlayerId, UCard* InCard);

	/**
	 * Let local player use card by instance id.
	 * @param InInstanceId		Instance Id of card that to be used.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card")
	static void UseCardByInstanceId(const UObject* WorldContextObject, int32 InInstanceId);

	/**
	 * Get card count of specified player.
	 * @param InPlayerId		Player id to get card count.
	 * @return					Total card count of player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card")
	static int32 GetPlayerCardNums(const UObject* WorldContextObject, int32 InPlayerId);

	/**
	 * Add specified game widget to player with given id.
	 * @param WidgetName		Name of widget to show.
	 * @param InPlayerId		Id of player to add widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void AddWidgetByPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Show specified game widget to player with given id.
	 * @param WidgetName		Name of widget to show.
	 * @param InPlayerId		Id of player to change visibility.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void ShowWidgetByPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Hide specified game widget to player with given id.
	 * @param WidgetName		Name of widget to hide.
	 * @param InPlayerId		Id of player to hide widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void HideWidgetByPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Remove specified game widget from player with given id.
	 * @param WidgetName		Name of widget to remove.
	 * @param InPlayerId		Id of player to remove widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void RemoveWidgetPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Create damage display item to player with given id.
	 * @param DisplayParams		Display parameter set.
	 * @param InPlayerId		Id of player to show.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI")
	static void CreateDamageDisplayByPlayerId(const UObject* WorldContextObject, const FDamageDisplayParams& DisplayParams, int32 InPlayerId);

	/**
	 * Get the total count of alive player in current game.
	 * @return			Number of players are still alive.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static int32 GetAlivePlayerNum(const UObject* WorldContextObject);

	/**
	 * Get the id list of alive players.
	 * @return			Id list of alive player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static TArray<int32> GetAlivePlayerId(const UObject* WorldContextObject);

	/**
	 * Spawn game effect with given id at specified location on every connected client.
	 * @param EffectId			Id of game effect to spawn.
	 * @param Pos				Spawn position.
	 * @param Rot				Spawn rotation.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Effect")
	static AGameEffectBase* SpawnGameEffectAtPos(const UObject* WorldContextObject, int32 EffectId, FVector Pos,
	                                             FRotator Rot);

	/**
	 * Apply damage to given actor.
	 * @param DamageId			Id of damage to apply.
	 * @param Target			Target actor to receive damage.
	 * @param BaseDamage		Base damage value.
	 * @param Causer			Damage source.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void DamageActor(const UObject* WorldContextObject, int32 DamageId, AActor* Target, float BaseDamage, AActor* Causer);

	/**
	 * Apply damage to all actors in given sphere.
	 * @param DamageId			Id of damage to apply.
	 * @param Origin			Target location.
	 * @param Radius			Sphere radius.
	 * @param BaseDamage		Base damage value.
	 * @param Causer			Damage source.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void DamageArea(const UObject* WorldContextObject, int32 DamageId, const FVector& Origin, float Radius, float BaseDamage, AActor* Causer);

	/**
	 * Let character of specified player move to given position.
	 * @param PlayerId			Id of character owner.
	 * @param TargetPosition	Target position.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void MovePlayerCharacterToPosition(const UObject* WorldContextObject, int32 PlayerId, FVector TargetPosition);

	/**
	 * Set specified actor invincible/vulnerable.
	 * @param Actor				Target actor to set.
	 * @param InvincibleCauser			Invincible or vulnerable.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void SetActorInvincible(AActor* Actor, bool InValue, UObject* InvincibleCauser);

	/**
	 * Get a random point in navigation grid.
	 * @return Location of random point.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static FVector GetRandomPointInNavigationArea(const UObject* WorldContextObject);

	/**
	 * Get a random player character in current game.
	 * @param Count				Total count of target to get.
	 * @param bAllowDuplicated	Whether if the result can duplicated.
	 * @return Random character list.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static TArray<AActor*> GetRandomCharacterInGame(const UObject* WorldContextObject, int32 Count = 1, bool bAllowDuplicated = false);

	/**
	 * Let character attack given target.
	 * @param InCharacter		Character that start attacking.
	 * @param Target			Target actor to attack.
	 * @return The slot of weapon that used to attack target.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static EWeaponSlotType CharacterAttack(ATopDownStylePlayerCharacter* InCharacter, AActor* Target);
protected:
	/**
	 * Do specified function on every player state in current game meet the id mask.
	 */
	static void ForEachPlayerStateDo(const UObject* WorldContextObject, TFunction<void(ATopDownStylePlayerState*)> ExecuteFunc, int32 PlayerIdMask = -1);

	/**
	 * Do specified function on every player controller in current game meet the id mask.
	 */
	static void ForEachPlayerControllerDo(const UObject* WorldContextObject, TFunction<void(ATopDownStylePlayerController*)> ExecuteFunc, int32 PlayerIdMask = -1);
};
