// Dog Fight Game Code By CYM.


#include "UI/Widget/AbilitySelectWindowWidget.h"

void UAbilitySelectWindowWidget::SetDisplayAbilities(TArray<FAbilityDisplayInfo> AbilityDisplayInfos)
{
	K2_DisplayAbilitiesChanged(AbilityDisplayInfos);
}

void UAbilitySelectWindowWidget::SelectAbilityAtIndex(int32 Index)
{
	OnAbilitySelected.Broadcast(Index);
}
