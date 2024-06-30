// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameLobbyFunctionLibrary.generated.h"

class AGameLobbyGameMode;
class AGameLobbyGameState;
class AGameLobbyPlayerState;
class AGameLobbyPlayerController;
class UCommonSessionSubsystem;
class UGameplayExperience;

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UGameLobbyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Get all player states of current game lobby.
	 * @return List of player state in current lobby.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary", meta=(WorldContext="WorldContext"))
	static TArray<AGameLobbyPlayerState*> GetAllGameLobbyPlayerState(UObject* WorldContext);

	/**
	 * Get game lobby player controller of local player.
	 * @return Game lobby player controller of local player.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary", meta=(WorldContext="WorldContext"))
	static AGameLobbyPlayerController* GetLocalGameLobbyPlayerController(UObject* WorldContext);

	/**
	 * Get game lobby player state of local player.
	 * @return Game lobby player state of local player.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary", meta=(WorldContext="WorldContext"))
	static AGameLobbyPlayerState* GetLocalGameLobbyPlayerState(UObject* WorldContext);

	/**
	 * Get game state of current game lobby.
	 * @return Game state of current game lobby.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary", meta=(WorldContext="WorldContext"))
	static AGameLobbyGameState* GetCurrentLobbyGameState(UObject* WorldContext);

	/**
	 * Get available common session subsystem.
	 * @return Common session subsystem.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary", meta=(WorldContext="WorldContext"))
	static UCommonSessionSubsystem* GetCommonSessionSubSystem(UObject* WorldContext);

	/**
	 * Dismiss current game lobby.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary", meta=(WorldContext="WorldContext"))
	static void DismissGameLobby(UObject* WorldContext);

	/**
	 * Check if given id belongs local player.
	 * @param PlayerId PlayerId to check with.
	 * @return Whether given Id represents local player.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary", meta=(WorldContext="WorldContext"))
	static bool IsLocalPlayer(UObject* WorldContext, int32 PlayerId);
};
