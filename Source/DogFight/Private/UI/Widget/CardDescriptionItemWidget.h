// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizableCardTypes.h"
#include "CardDescriptionItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class UCardDescriptionItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void ApplyItemInfo(const FCardDescriptionItemInfo& ItemInfo);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta=(DisplayName="OnApplyItemInfo"))
	void K2_OnApplyItemInfo();

	/** Get localized text of this item. */
	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FText GetLocalizedText() const;

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FText GetLocalizedExtraArgumentText(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FUpgradablePropertyDisplayInfo GetPropertyDisplayInfo(FString PropertyName);

protected:

	FCardDescriptionItemInfo OwningItemInfo;
};
