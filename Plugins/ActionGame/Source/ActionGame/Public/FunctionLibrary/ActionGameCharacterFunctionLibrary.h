// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActionGameCharacterFunctionLibrary.generated.h"

class AActionGameCharacter;

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionGameCharacterFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Calculate the minimum distance for two character collide each other.
	 * @param A First character to calculate distance.
	 * @param B Second character to calculate distance.
	 * @return Min distance for two characters collide each other.
	 */
	UFUNCTION(BlueprintCallable, Category = "ActionGameCharacter")
	static float GetCollideDistance(AActionGameCharacter* A, AActionGameCharacter* B);
};
