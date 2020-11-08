// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizableCardTypes.h"
#include "UpgradablePropertyTooltipsWidget.generated.h"

/**
 * 
 */
UCLASS()
class UUpgradablePropertyTooltipsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FText GetLocalizedText(const FUpgradablePropertyDisplayInfo& DisplayInfo) const;

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FText GetLocalizedArgument(const FUpgradablePropertyDisplayInfo& DisplayInfo, int32 Index) const;
};
