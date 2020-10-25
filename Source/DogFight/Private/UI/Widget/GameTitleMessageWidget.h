// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightTypes.h"
#include "Blueprint/UserWidget.h"
#include "GameTitleMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameTitleMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void DisplayTitleMessage(FGameTitleMessage TitleMessage);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnTitleMessageReceived(const FText& GameTitleMessage);
};
