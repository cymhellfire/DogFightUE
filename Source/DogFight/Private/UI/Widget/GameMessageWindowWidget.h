// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "DogFightTypes.h"
#include "UI/Widget/SlotWidget.h"
#include "GameMessageWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameMessageWindowWidget : public USlotWidget
{
	GENERATED_BODY()

public:
	/** Fired when a new GameMessage is received. */
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnGameMessageReceived(EGameMessageType Type, const FText& GameMessage, const FString& MessageSource);

	void AddGameMessage(FGameMessage Message);

};
