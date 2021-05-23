// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInGameHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddWidgetToSlotByName(UUserWidget* WidgetToAdd, FName SlotName);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta=(DisplayName="Add Widget To Slot"))
	void K2_AddWidgetToSlotByName(UUserWidget* WidgetToAdd, FName SlotName);
};
