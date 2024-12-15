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
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static UGameplayDataSubsystem* GetGameplayDataSubsystem(UObject* WorldContextObject);

	/**
	 * Get PlayerState by player id.
	 * @param InPlayerId		Id of player that acquire state for.
	 * @return					The PlayerState of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static APlayerState* GetPlayerStateById(UObject* WorldContextObject, int32 InPlayerId);

	/**
	 * Get PlayerController by player id.
	 * @param InPlayerId		Id of player that acquire controller for.
	 * @return					The PlayerController of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static ATopDownStylePlayerController* GetPlayerControllerById(UObject* WorldContextObject, int32 InPlayerId);

	/**
	 * Get character pawn of specified player.
	 * @param InPlayerId		Id of player that acquire character for.
	 * @return					The player character of corresponding player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static ATopDownStylePlayerCharacter* GetPlayerCharacterById(UObject* WorldContextObject, int32 InPlayerId);

	/**
	 * Let specified pawn become its owner's controlling pawn.
	 * @param InPawn			Pawn instance to operate.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void SetControllingPawnToOwnerPlayer(ATopDownStylePlayerCharacter* InPawn);

	/**
	 * Given player specified card.
	 * @param InPlayerId		Id of player that cards given to.
	 * @param InCard			The card that given to player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card", meta=(WorldContext="WorldContextObject"))
	static void DispatchCardToPlayer(UObject* WorldContextObject, int32 InPlayerId, UCard* InCard);

	/**
	 * Given character specified card.
	 * @param InCharacter		Character pawn that cards given to.
	 * @param InCard			The card that given to character.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card", meta=(WorldContext="WorldContextObject"))
	static bool DispatchCardToCharacter(UObject* WorldContextObject, ATopDownStylePlayerCharacter* InCharacter, UCard* InCard);

	/**
	 * Let local player use card by instance id.
	 * @param InInstanceId		Instance Id of card that to be used.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card", meta=(WorldContext="WorldContextObject"))
	static void UseCardByInstanceId(UObject* WorldContextObject, int32 InInstanceId);

	/**
	 * Get card count of specified character.
	 * @param InPawn			Character to get card count.
	 * @return					Total card count.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Card", meta=(WorldContext="WorldContextObject"))
	static int32 GetCharacterCardNums(ATopDownStylePlayerCharacter* InPawn);

	/**
	 * Add specified game widget to player with given id.
	 * @param WidgetName		Name of widget to show.
	 * @param InPlayerId		Id of player to add widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI", meta=(WorldContext="WorldContextObject"))
	static void AddWidgetByPlayerId(UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Show specified game widget to player with given id.
	 * @param WidgetName		Name of widget to show.
	 * @param InPlayerId		Id of player to change visibility.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI", meta=(WorldContext="WorldContextObject"))
	static void ShowWidgetByPlayerId(UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Hide specified game widget to player with given id.
	 * @param WidgetName		Name of widget to hide.
	 * @param InPlayerId		Id of player to hide widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI", meta=(WorldContext="WorldContextObject"))
	static void HideWidgetByPlayerId(UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Remove specified game widget from player with given id.
	 * @param WidgetName		Name of widget to remove.
	 * @param InPlayerId		Id of player to remove widget.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI", meta=(WorldContext="WorldContextObject"))
	static void RemoveWidgetPlayerId(UObject* WorldContextObject, FString WidgetName, int32 InPlayerId);

	/**
	 * Create damage display item to player with given id.
	 * @param DisplayParams		Display parameter set.
	 * @param InPlayerId		Id of player to show.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|UI", meta=(WorldContext="WorldContextObject"))
	static void CreateDamageDisplayByPlayerId(UObject* WorldContextObject, const FDamageDisplayParams& DisplayParams, int32 InPlayerId);

	/**
	 * Get the total count of alive player in current game.
	 * @return			Number of players are still alive.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static int32 GetAlivePlayerNum(UObject* WorldContextObject);

	/**
	 * Get the id list of alive players.
	 * @return			Id list of alive player.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static TArray<int32> GetAlivePlayerId(UObject* WorldContextObject);

	/**
	 * Spawn game effect with given id at specified location on every connected client.
	 * @param EffectId			Id of game effect to spawn.
	 * @param Pos				Spawn position.
	 * @param Rot				Spawn rotation.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay|Effect", meta=(WorldContext="WorldContextObject"))
	static AGameEffectBase* SpawnGameEffectAtPos(UObject* WorldContextObject, int32 EffectId, FVector Pos,
	                                             FRotator Rot);

	/**
	 * Apply damage to given actor.
	 * @param DamageId			Id of damage to apply.
	 * @param Target			Target actor to receive damage.
	 * @param BaseDamage		Base damage value.
	 * @param Causer			Damage source.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static void DamageActor(UObject* WorldContextObject, int32 DamageId, AActor* Target, float BaseDamage, AActor* Causer);

	/**
	 * Apply damage to all actors in given sphere.
	 * @param DamageId			Id of damage to apply.
	 * @param Origin			Target location.
	 * @param Radius			Sphere radius.
	 * @param BaseDamage		Base damage value.
	 * @param Causer			Damage source.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static void DamageArea(UObject* WorldContextObject, int32 DamageId, const FVector& Origin, float Radius, float BaseDamage, AActor* Causer);

	/**
	 * Let character of specified player move to given position.
	 * @param PlayerId			Id of character owner.
	 * @param TargetPosition	Target position.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static void MovePlayerCharacterToPosition(UObject* WorldContextObject, int32 PlayerId, FVector TargetPosition);

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
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static FVector GetRandomPointInNavigationArea(UObject* WorldContextObject);

	/**
	 * Get a random player character in current game.
	 * @param Count				Total count of target to get.
	 * @param bAllowDuplicated	Whether if the result can duplicated.
	 * @return Random character list.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay", meta=(WorldContext="WorldContextObject"))
	static TArray<AActor*> GetRandomCharacterInGame(UObject* WorldContextObject, int32 Count = 1, bool bAllowDuplicated = false);

	/**
	 * Let character perform weapon input with given target.
	 * @param InCharacter		Character that perform action.
	 * @param Target			Target actor to perform with.
	 * @param InputType			Weapon input type.
	 * @return The slot of weapon that used to execute input.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static EWeaponSlotType PerformWeaponInputWithTarget(ATopDownStylePlayerCharacter* InCharacter, AActor* Target, EWeaponActionInput InputType);
protected:
	/**
	 * Do specified function on every player state in current game meet the id mask.
	 */
	static void ForEachPlayerStateDo(UObject* WorldContextObject, TFunction<void(ATopDownStylePlayerState*)> ExecuteFunc, int32 PlayerIdMask = -1);

	/**
	 * Do specified function on every player controller in current game meet the id mask.
	 */
	static void ForEachPlayerControllerDo(UObject* WorldContextObject, TFunction<void(ATopDownStylePlayerController*)> ExecuteFunc, int32 PlayerIdMask = -1);
};
