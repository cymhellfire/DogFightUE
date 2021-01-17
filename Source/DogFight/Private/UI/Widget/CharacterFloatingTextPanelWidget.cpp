// Dog Fight Game Code By CYM.


#include "UI/Widget/CharacterFloatingTextPanelWidget.h"

#include "Game/DogFightDamageType.h"

void UCharacterFloatingTextPanelWidget::AddFloatText(FText NewText)
{
	// Invoke Blueprint function
	K2_OnNewFloatingTextAdded(NewText);
}

void UCharacterFloatingTextPanelWidget::AddDamageText(float Damage, UDamageType* DamageType)
{
	FString DamageText = FString::Printf(TEXT("%.2f"), Damage);
	// Check damage type and format the text
	if (UDogFightDamageType* DogFightDamage = Cast<UDogFightDamageType>(DamageType))
	{
		const FString StyleName = DogFightDamage->DamageStyleName;
		if (!StyleName.IsEmpty())
		{
			DamageText = FString::Printf(TEXT("<%s>%s</>"), *StyleName, *DamageText);
		}
	}

	K2_OnNewFloatingTextAdded(FText::FromString(DamageText));
}
