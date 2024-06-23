// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameSettingsFunctionLibrary.generated.h"

class USaveGameSubsystem;

/**
 * Function library for game settings.
 */
UCLASS()
class DOGFIGHTGAME_API UGameSettingsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Manually save current game global profile.
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings", meta=(WorldContext="WorldContextObject"))
	static void SaveGameSettings(UObject* WorldContextObject);

	/**
	 * Set the player name in game.
	 * @param InName New player name to use.
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings", meta=(WorldContext="WorldContextObject"))
	static void SetPlayerName(UObject* WorldContextObject, const FString& InName);

	/**
	 * Get the player name in game.
	 * @return Player name to use.
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings", meta=(WorldContext="WorldContextObject"))
	static FString GetPlayerName(UObject* WorldContextObject);
};
