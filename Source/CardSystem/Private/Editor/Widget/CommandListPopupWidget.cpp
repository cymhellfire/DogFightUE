#include "Editor/Widget/CommandListPopupWidget.h"

#include "CardSystem.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Editor/CardCommandVisual.h"
#include "Editor/Widget/CommandListItemWidget.h"

bool UCommandListPopupWidget::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (CommandListView)
	{
		CommandListItemSelectedHandle = CommandListView->OnItemSelectionChanged().AddUObject(this, &UCommandListPopupWidget::OnCommandListItemSelectedChanged);
	}

	if (CommandListViewTrigger)
	{
		CommandListViewTrigger->OnClicked.AddDynamic(this, &UCommandListPopupWidget::OnCommandListTriggerClicked);
	}

	return true;
}

void UCommandListPopupWidget::BeginDestroy()
{
	Super::BeginDestroy();

	if (CommandListView)
	{
		CommandListView->OnItemClicked().Remove(CommandListItemSelectedHandle);
	}

	if (CommandListViewTrigger)
	{
		CommandListViewTrigger->OnClicked.RemoveDynamic(this, &UCommandListPopupWidget::OnCommandListTriggerClicked);
	}

	OnCommandItemConfirm.Clear();
}

void UCommandListPopupWidget::SetClassList(TArray<UCardCommandVisual*> InClassList)
{
	// Clear existing items
	CommandListView->ClearListItems();

	for (UCardCommandVisual* Item : InClassList)
	{
		CommandListView->AddItem(Item);
	}
}

void UCommandListPopupWidget::ShowCommandListView()
{
	if (CommandListViewContainer->IsVisible())
		return;

	CommandListViewContainer->SetVisibility(ESlateVisibility::Visible);

	bCommandListOpened = true;
}

void UCommandListPopupWidget::HideCommandListView()
{
	if (!CommandListViewContainer->IsVisible())
		return;

	CommandListViewContainer->SetVisibility(ESlateVisibility::Collapsed);

	bCommandListOpened = false;
}

void UCommandListPopupWidget::OnCommandListItemSelectedChanged(UObject* Item)
{
	UE_LOG(LogCardSystem, Log, TEXT("[PopupWidget] %s selected."), *Item->GetName());

	if (SelectedItemVisual)
	{
		if (UCardCommandVisual* CardCommandVisual = Cast<UCardCommandVisual>(Item))
		{
			SelectedItemVisual->InitializeWithCommand(CardCommandVisual);

			CommandDescTextBlock->SetText(CardCommandVisual->GetCommandDesc());

			SelectedCommandId = CardCommandVisual->GetCommandId();
		}
		else
		{
			SelectedCommandId = -1;
		}

		HideCommandListView();
	}
}

void UCommandListPopupWidget::OnCommandListTriggerClicked()
{
	if (bCommandListOpened)
	{
		HideCommandListView();
	}
	else
	{
		ShowCommandListView();
	}
}

void UCommandListPopupWidget::OnConfirmButtonClicked()
{
	Super::OnConfirmButtonClicked();
	UE_LOG(LogCardSystem, Log, TEXT("[PopupWidget] Confirm!"));

	OnCommandItemConfirm.Broadcast(SelectedCommandId);
}

void UCommandListPopupWidget::OnCancelButtonClicked()
{
	Super::OnCancelButtonClicked();
	UE_LOG(LogCardSystem, Log, TEXT("[PopupWidget] Cancel!"));

	OnCommandItemConfirm.Broadcast(-1);
}
