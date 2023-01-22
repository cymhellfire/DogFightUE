#pragma once

#include "LuaIntegrationFunctionLibrary.generated.h"

UCLASS(BlueprintType)
class LUAINTEGRATION_API ULuaIntegrationFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Get the current active world.
	 * @return Current playing world.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static UWorld* GetCurrentWorld();

	/**
	 * Get the game instance.
	 * @return Current game instance.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static UGameInstance* GetGameInstance();

	/**
	 * Get the player controller of first local player.
	 * @return			Player controller.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static APlayerController* GetFirstLocalPlayerController();

	/**
	 * Load the class from given path.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static UClass* LoadClassByPath(FString InPath);

	/**
	 * Check whether the object is derived from specified class.
	 * @param InObject Object to check with.
	 * @param InClass Class that cast to.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static bool IsDerivedFrom(UObject* InObject, UClass* InClass);
};
