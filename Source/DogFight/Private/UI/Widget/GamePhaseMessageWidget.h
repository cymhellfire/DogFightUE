// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePhaseMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGamePhaseMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Fire when the game phase to display has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnGamePhaseMessageChanged(const FName& GamePhase);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnCountdownContentChanged();

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FString GetCountdownContentString() const { return CountdownContent; }

	/** Set the game phase to display. */
	void SetGamePhase(const FName& GamePhase);

	/** Set the countdown context string to display. */
	void SetCountdownContent(const FString& NewContext);

protected:

	FString CountdownContent;
};
