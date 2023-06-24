// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameLobbyFunctionLibrary.generated.h"

class AGameLobbyGameMode;
class AGameLobbyGameState;
class AGameLobbyPlayerState;
class AGameLobbyPlayerController;

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
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary")
	static TArray<AGameLobbyPlayerState*> GetAllGameLobbyPlayerState(const UObject* WorldContext);

	/**
	 * Get game lobby player controller of local player.
	 * @return Game lobby player controller of local player.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary")
	static AGameLobbyPlayerController* GetLocalGameLobbyPlayerController(const UObject* WorldContext);

	/**
	 * Get game lobby player state of local player.
	 * @return Game lobby player state of local player.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary")
	static AGameLobbyPlayerState* GetLocalGameLobbyPlayerState(const UObject* WorldContext);

	/**
	 * Get game state of current game lobby.
	 * @return Game state of current game lobby.
	 */
	UFUNCTION(BlueprintCallable, Category="GameLobbyFunctionLibrary")
	static AGameLobbyGameState* GetCurrentLobbyGameState(const UObject* WorldContext);
};
