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
	static UWorld* GetCurrentWorld(const UObject* WorldContextObject);

	/**
	 * Get the game instance.
	 * @return Current game instance.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static UGameInstance* GetGameInstance(const UObject* WorldContextObject);

	/**
	 * Get the player controller of first local player.
	 * @return			Player controller.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static APlayerController* GetFirstLocalPlayerController(const UObject* WorldContextObject);

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

	/**
	 * Get the localized string by specified key and string table.
	 * @param InTable		Name of the container string table.
	 * @param InKey			Key of the localized string.
	 * @return				Localized string based on current culture.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static FText GetLocalizedString(const UObject* WorldContextObject, const FString& InTable, const FString& InKey);

	/**
	 * Get the localized string by specified key and string table, and format with given arguments.
	 * @param InTable		Name of the container string table.
	 * @param InKey			Key of the localized string.
	 * @param ParamList		List of all arguments used by formatting.
	 * @return				Localized string based on current culture.
	 */
	UFUNCTION(BlueprintCallable, Category="LuaIntegrationFunction")
	static FText GetLocalizedStringWithParam(const UObject* WorldContextObject, const FString& InTable, const FString& InKey, const TArray<FString>& ParamList);
};
