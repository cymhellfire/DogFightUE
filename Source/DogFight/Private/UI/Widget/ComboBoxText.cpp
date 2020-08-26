// Dog Fight Game Code By CYM.


#include "ComboBoxText.h"

TSharedRef<SWidget> UComboBoxText::HandleGenerateWidget(::TSharedPtr<FString, ESPMode::Fast> Item) const
{
	FString StringItem = Item.IsValid() ? *Item : FString();

	// Call the user's delegate to see if they want to generate a custom widget bound to the data source.
	if ( !IsDesignTime() && OnGenerateWidgetEvent.IsBound() )
	{
		UWidget* Widget = OnGenerateWidgetEvent.Execute(StringItem);
		if ( Widget != nullptr )
		{
			return Widget->TakeWidget();
		}
	}

	// Try to create a localized item if possible
	return SNew(STextBlock)
        .Text(LocalizationStringTable.IsValid() ? FText::FromStringTable(LocalizationStringTable, StringItem) : FText::FromString(StringItem))
        .Font(Font);
}
