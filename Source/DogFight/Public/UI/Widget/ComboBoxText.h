// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "ComboBoxText.generated.h"

/**
 * Localized version of ComboBox.
 */
UCLASS(meta=(DisplayName="ComboBox (Text)"))
class DOGFIGHT_API UComboBoxText : public UComboBoxString
{
	GENERATED_BODY()

private:

	/** The localization source of this combo box options. */
	UPROPERTY(EditAnywhere, Category=Content)
	FName LocalizationStringTable;

protected:

	/** Override text box generation for localization. */
	virtual TSharedRef<SWidget> HandleGenerateWidget(TSharedPtr<FString> Item) const override;
};
