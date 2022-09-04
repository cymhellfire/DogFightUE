#pragma once

#include "CoreMinimal.h"

struct FTextInputPopupParameter
{
	FText Title;
	FText HintText;
	FText DefaultText;
	FVector2D PopupSize;

	FTextInputPopupParameter()
	{
		Title = FText::GetEmpty();
		HintText = FText::GetEmpty();
		DefaultText = FText::GetEmpty();
		PopupSize = FVector2D(650, 450);
	}
};
