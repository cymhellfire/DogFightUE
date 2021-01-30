// Dog Fight Game Code By CYM.


#include "UI/Widget/CharacterFloatingTextPanelWidget.h"

#include "Game/DogFightDamageType.h"

UCharacterFloatingTextPanelWidget::UCharacterFloatingTextPanelWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MinIntervalBetweenText = 0.2f;
}

void UCharacterFloatingTextPanelWidget::AddFloatText(FText NewText)
{
	if (FloatingTextIntervalTimerHandle.IsValid())
	{
		FloatingTextQueue.Add(NewText);
	}
	else
	{
		// Invoke Blueprint function
		K2_OnNewFloatingTextAdded(NewText);

		GetWorld()->GetTimerManager().SetTimer(FloatingTextIntervalTimerHandle, this,
			&UCharacterFloatingTextPanelWidget::OnIntervalTimerExpired, MinIntervalBetweenText, true);
	}
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
			DamageText = FString::Printf(TEXT("<img id=\"%s\"/><%s>%s</>"), *DogFightDamage->DamageTypeName.LocalizeKey, *StyleName, *DamageText);
		}
	}

	if (FloatingTextIntervalTimerHandle.IsValid())
	{
		FloatingTextQueue.Add(FText::FromString(DamageText));
	}
	else
	{
		K2_OnNewFloatingTextAdded(FText::FromString(DamageText));

		GetWorld()->GetTimerManager().SetTimer(FloatingTextIntervalTimerHandle, this,
			&UCharacterFloatingTextPanelWidget::OnIntervalTimerExpired, MinIntervalBetweenText, true);
	}
}

void UCharacterFloatingTextPanelWidget::OnIntervalTimerExpired()
{
	if (FloatingTextQueue.Num() > 0)
	{
		K2_OnNewFloatingTextAdded(FloatingTextQueue[0]);

		FloatingTextQueue.RemoveAt(0);
	}

	if (FloatingTextQueue.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(FloatingTextIntervalTimerHandle);
	}
}
