// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DogFightGameplayLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UDogFightGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="DogFight|Actor")
	static void TeleportActor(const AActor* Actor, const FVector& DestPos);
};
