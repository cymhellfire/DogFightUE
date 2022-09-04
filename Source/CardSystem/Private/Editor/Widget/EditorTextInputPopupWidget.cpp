#include "Editor/Widget/EditorTextInputPopupWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"

void UEditorTextInputPopupWidget::OnConfirmButtonClicked()
{
	if (InputTextBlock)
	{
		OnInputTextConfirmed.Broadcast(InputTextBlock->GetText());
	}

	Super::OnConfirmButtonClicked();
}

void UEditorTextInputPopupWidget::Close()
{
	Super::Close();

	// Clear listener
	OnInputTextConfirmed.Clear();
}

void UEditorTextInputPopupWidget::ApplyParam(const FTextInputPopupParameter& InParam)
{
	if (TitleTextBlock)
	{
		TitleTextBlock->SetText(InParam.Title);
	}

	if (PopupSizeBox)
	{
		FVector2D NewSize = InParam.PopupSize;
		PopupSizeBox->bOverride_HeightOverride = true;
		PopupSizeBox->bOverride_WidthOverride = true;
		PopupSizeBox->SetHeightOverride(NewSize.Y);
		PopupSizeBox->SetWidthOverride(NewSize.X);
	}

	if (InputTextBlock)
	{
		InputTextBlock->SetText(InParam.DefaultText);
		InputTextBlock->SetHintText(InParam.HintText);
	}
}
