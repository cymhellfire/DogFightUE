// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystem/InGameStatistics/GameLobbyPlayerAvatarId.h"
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
	UFUNCTION(BlueprintCallable, Category="GameSettings")
	static void SaveGameSettings();

	/**
	 * Set the player name in game.
	 * @param InName New player name to use.
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings")
	static void SetPlayerName(const FString& InName);

	/**
	 * Get the player name in game.
	 * @return Player name to use.
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings")
	static FString GetPlayerName();

	/**
	 * Record avatar id.
	 * @param InId Avatar id to save.
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings")
	static void SetLastAvatarId(const FGameLobbyPlayerAvatarId& InId);

	/**
	 * Get the avatar id last selected.
	 * @return Last selected avatar id.
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings")
	static FGameLobbyPlayerAvatarId GetLastAvatarId(); 
};
