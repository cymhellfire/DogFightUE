// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightTypes.h"
#include "UI/Widget/SlotWidget.h"
#include "GameTitleMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameTitleMessageWidget : public USlotWidget
{
	GENERATED_BODY()

public:

	void DisplayTitleMessage(FGameTitleMessage TitleMessage);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnTitleMessageReceived(const FText& GameTitleMessage);
};
