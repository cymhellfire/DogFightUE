// Dog Fight Game Code By CYM.


#include "UI/Widget/InGameHudWidget.h"

void UInGameHudWidget::AddWidgetToSlotByName(UUserWidget* WidgetToAdd, FName SlotName)
{
	K2_AddWidgetToSlotByName(WidgetToAdd, SlotName);
}
