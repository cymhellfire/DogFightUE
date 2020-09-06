// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FrameworkLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UFrameworkLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Get the SaveGameManager instance. */
	UFUNCTION(BlueprintCallable, Category="DogFight|Framework", meta=(WorldContext="WorldContextObject"))
	static class USaveGameManager* GetSaveGameManager(const UObject* WorldContextObject);
};
