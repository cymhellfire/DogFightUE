#include "Editor/Widget/FunctionListItemWidget.h"

#include "Components/RichTextBlock.h"
#include "Editor/CardFunctionVisual.h"

void UFunctionListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UCardFunctionVisual* FunctionVisual = Cast<UCardFunctionVisual>(ListItemObject);
	if (FunctionVisual)
	{
		InitializeWithVisual(FunctionVisual);
	}
}

void UFunctionListItemWidget::InitializeWithVisual(UCardFunctionVisual* FunctionVisual)
{
	if (FunctionNameTextBlock)
	{
		FunctionNameTextBlock->SetText(FText::FromString(FunctionVisual->GetFunctionName()));
	}
}
