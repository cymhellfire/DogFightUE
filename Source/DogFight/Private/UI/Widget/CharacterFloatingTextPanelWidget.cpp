// Dog Fight Game Code By CYM.


#include "UI/Widget/CharacterFloatingTextPanelWidget.h"

void UCharacterFloatingTextPanelWidget::AddFloatText(FText NewText)
{
	// Invoke Blueprint function
	K2_OnNewFloatingTextAdded(NewText);
}

void UCharacterFloatingTextPanelWidget::AddDamageText(float Damage)
{
	K2_OnNewFloatingTextAdded(FText::FromString(FString::Printf(TEXT("%f"), Damage)));
}
