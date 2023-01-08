#pragma once

#include "CoreMinimal.h"
#include "CommonGameFlowFunctionLibrary.generated.h"

class ATopDownStylePlayerController;

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
	static TArray<ATopDownStylePlayerController*> GetAllPlayerControllers();

	UFUNCTION(BlueprintCallable, Category="CommonGameFlow")
	static void SpawnPlayerCharacterPawn(ATopDownStylePlayerController* Controller);
};
