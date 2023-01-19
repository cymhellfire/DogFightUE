#pragma once

#include "CommonFunctionLibraryBase.h"
#include "CommonGameplayFunctionLibrary.generated.h"

class UCard;
class ATopDownStylePlayerController;

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
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void DispatchCardToPlayer(int32 InPlayerId, UCard* InCard);

	/**
	 * Let local player use card by instance id.
	 * @param InInstanceId		Instance Id of card that to be used.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameplay")
	static void UseCardByInstanceId(int32 InInstanceId);
};
