// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DogFightSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFRAMEWORK_API UDogFightSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category=Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category=Basic)
	uint32 Money;

	UPROPERTY(VisibleAnywhere, Category=Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category=Basic)
	uint32 UserIndex;

	UFUNCTION(BlueprintCallable, Category="SaveGame|DogFight")
	void SetMoney(int32 InMoney);

	UDogFightSaveGame();
};
