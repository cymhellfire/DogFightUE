#include "Editor/Widget/CardFunctionTabItemWidget.h"

#include "Components/RichTextBlock.h"
#include "Editor/CardFunctionVisual.h"
#include "Editor/EditingCardFunction.h"

void UCardFunctionTabItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	if (UCardFunctionVisual* FunctionVisual = Cast<UCardFunctionVisual>(ListItemObject))
	{
		InitializeWithFunction(FunctionVisual->GetFunction());
	}
}

void UCardFunctionTabItemWidget::InitializeWithFunction(TWeakPtr<FEditingCardFunction> InFunction)
{
	if (!InFunction.IsValid() || BoundFunction == InFunction)
	{
		return;
	}

	if (BoundFunction.IsValid())
	{
		ClearFunctionListener(BoundFunction.Pin().Get());
	}

	BindFunctionListener(InFunction.Pin().Get());
	BoundFunction = InFunction;

	if (FunctionNameTextBlock)
	{
		FunctionNameTextBlock->SetText(FText::FromString(InFunction.Pin()->GetName()));
	}
}

void UCardFunctionTabItemWidget::BindFunctionListener(FEditingCardFunction* InFunction)
{
	FunctionNameChangedHandle = InFunction->OnEditingFunctionNameChanged.AddUObject(this, &UCardFunctionTabItemWidget::OnFunctionNameChanged);
}

void UCardFunctionTabItemWidget::ClearFunctionListener(FEditingCardFunction* InFunction)
{
	InFunction->OnEditingFunctionNameChanged.Remove(FunctionNameChangedHandle);
}

void UCardFunctionTabItemWidget::OnFunctionNameChanged(FString InName)
{
	if (FunctionNameTextBlock)
	{
		FunctionNameTextBlock->SetText(FText::FromString(InName));
	}
}

void UCardFunctionTabItemWidget::OnCloseTabButtonClicked()
{
	
}
