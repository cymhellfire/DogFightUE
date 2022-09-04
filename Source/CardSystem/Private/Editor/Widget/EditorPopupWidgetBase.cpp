#include "Editor/Widget/EditorPopupWidgetBase.h"

#include "Components/Button.h"

bool UEditorPopupWidgetBase::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UEditorPopupWidgetBase::OnConfirmButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UEditorPopupWidgetBase::OnCancelButtonClicked);
	}

	return true;
}

void UEditorPopupWidgetBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UEditorPopupWidgetBase::OnConfirmButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.RemoveDynamic(this, &UEditorPopupWidgetBase::OnCancelButtonClicked);
	}
}

void UEditorPopupWidgetBase::OnConfirmButtonClicked()
{
	Close();
}

void UEditorPopupWidgetBase::OnCancelButtonClicked()
{
	Close();
}

void UEditorPopupWidgetBase::Close()
{
	RemoveFromParent();
}
