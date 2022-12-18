#pragma once

#include "CoreMinimal.h"
#include "CommonGameFlowFunctionLibrary.generated.h"

class AFreeForAllPlayerController;

UCLASS()
class DOGFIGHTGAME_API UCommonGameFlowFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Get all player controller in current game.
	 *
	 * @return Array of all player controllers.
	 */
	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static TArray<AFreeForAllPlayerController*> GetAllPlayerControllers();

	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SpawnPlayerCharacterPawn(AFreeForAllPlayerController* Controller);
};
