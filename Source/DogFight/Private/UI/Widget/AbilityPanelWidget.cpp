// Dog Fight Game Code By CYM.


#include "UI/Widget/AbilityPanelWidget.h"

void UAbilityPanelWidget::AddAbility(FAbilityDisplayInfo NewAbilityInfo, int32 AbilitySlot)
{
	K2_AddAbility(NewAbilityInfo, AbilitySlot);
}

void UAbilityPanelWidget::RemoveAbility(int32 AbilityToRemove)
{
	K2_RemoveAbility(AbilityToRemove);
}

void UAbilityPanelWidget::UpdateAbilityCooldownAtSlot(int32 AbilitySlot, int32 CurrentCooldown)
{
	K2_UpdateAbilityCooldown(AbilitySlot, CurrentCooldown);
}

void UAbilityPanelWidget::UseAbilityAtSlot(int32 AbilitySlot)
{
	OnAbilitySelected.Broadcast(AbilitySlot);
}
