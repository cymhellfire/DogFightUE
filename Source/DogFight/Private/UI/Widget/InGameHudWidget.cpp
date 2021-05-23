// Dog Fight Game Code By CYM.


#include "UI/Widget/InGameHudWidget.h"
#include "UI/Widget/SlotWidget.h"

void UInGameHudWidget::AddWidgetToSlotByName(USlotWidget* WidgetToAdd)
{
	K2_AddWidgetToSlotByName(WidgetToAdd, WidgetToAdd->SlotToAdd);
}
