// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DogFightCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UDogFightCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UFUNCTION(Exec, BlueprintCallable, Category="Cheat Manager")
	void TestScript(FString FileName);
};
