// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "SaveGameManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTFRAMEWORK_API USaveGameManager : public UObject
{
	GENERATED_BODY()

public:
	/** Detect save game list. */
	UFUNCTION(Category="SaveGame|DogFight", BlueprintCallable)
	TArray<FString> GetAllSaveGame();

	/** Save game to specified slot. */
	UFUNCTION(Category="SaveGame|DogFight", BlueprintCallable)
	bool SaveAtSlot(FString PlayerName, FString OldSlotName);

	UFUNCTION(Category="SaveGame|DogFight", BlueprintCallable)
	void DeleteAtSlot(FString SlotName);

	/** Select the save game in specified slot. */
	UFUNCTION(Category="SaveGame|DogFight", BlueprintCallable)
	void SelectSaveGame(FString SlotName);

	/** Get current selected save game. */
	UFUNCTION(Category="SaveGame|DogFight", BlueprintCallable)
	FString GetSelectedSlot() { return SelectedSlotName; }

	UFUNCTION(Category="SaveGame|DogFight", BlueprintCallable)
	class UDogFightSaveGame* GetCurrentSaveGameInstance() { return CurrentSaveGameInstance; }

private:
	USaveGameManager(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	/** The SaveGameInstance current loaded. */
	class UDogFightSaveGame* CurrentSaveGameInstance;

	/** Cached SaveGameSlotName list from last refresh operation. */
	TArray<FString> CacheSlotList;

	/** The SlotName of current selected save game. */
	FString SelectedSlotName;

	void LoadOptionFile();

	void SetProperty(FString Key, FString Value);

	void SaveOptionFile();

	void LoadSaveGameAtSlot(FString SlotName);
};
