// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CharacterFloatingTextPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class UCharacterFloatingTextPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void AddFloatText(FText NewText);

	void AddDamageText(float Damage, UDamageType* DamageType);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta = (DisplayName = "OnNewFloatingTextAdded"))
	void K2_OnNewFloatingTextAdded(const FText& NewText);
};
