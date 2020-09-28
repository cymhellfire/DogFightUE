// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "DogFightTypes.h"
#include "Blueprint/UserWidget.h"
#include "GameMessageWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameMessageWindowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Fired when a new GameMessage is received. */
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnGameMessageReceived(EGameMessageType Type, const FText& GameMessage, const FString& MessageSource);

	void AddGameMessage(FGameMessage Message);

};
