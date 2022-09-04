#include "Editor/Widget/EditorMultiTabWidgetBase.h"

#include "CardSystem.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "Editor/Widget/EditorSelectableListItemBase.h"

bool UEditorMultiTabWidgetBase::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (TabItemListView)
	{
		TabItemListView->SetSelectionMode(ESelectionMode::Single);
		TabItemSelectionChangedHandle = TabItemListView->OnItemSelectionChanged().AddUObject(this, &UEditorMultiTabWidgetBase::OnTabItemSelectionChanged);
	}

	return true;
}

void UEditorMultiTabWidgetBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (TabItemListView)
	{
		TabItemListView->OnItemSelectionChanged().Remove(TabItemSelectionChangedHandle);
	}
}

void UEditorMultiTabWidgetBase::AddNewTab(UObject* Item)
{
	if (TabItemListView == nullptr)
	{
		return;
	}

	TabItemListView->AddItem(Item);
	if (UUserWidget* NewItemWidget = TabItemListView->GetEntryWidgetFromItem(Item))
	{
		InitializeTabItem(NewItemWidget, Item);
	}

	APlayerController* PC = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[EditorMultiTabWidget] Failed to get player controller"));
		return;
	}

	UUserWidget* NewTabWidget = CreateWidget(PC, TabWidgetClass);
	if (!NewTabWidget)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[EditorMultiTabWidget] Failed to create tab widget."));
		return;
	}

	InitializeTabWidget(NewTabWidget, Item);
	TabWidgetSwitcher->AddChild(NewTabWidget);

	// Select most recently added item
	SelectItem(Item);
}

void UEditorMultiTabWidgetBase::RemoveTab(UObject* Item)
{
	int32 ItemIndex = TabItemListView->GetIndexForItem(Item);
	if (ItemIndex == -1)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[EditorMultiTabWidget] Item is already removed."));
		return;
	}

	TabItemListView->RemoveItem(Item);
	TabWidgetSwitcher->RemoveChildAt(ItemIndex);

	// If current selected item removed
	if (ItemIndex == SelectedIndex)
	{
		SelectFirstTab();
	}
}

void UEditorMultiTabWidgetBase::SelectItem(UObject* Item)
{
	if (TabItemListView->GetIndexForItem(Item) == -1)
	{
		return;
	}

	TabItemListView->SetSelectedItem(Item);
}

void UEditorMultiTabWidgetBase::SelectFirstTab()
{
	if (TabItemListView->GetNumItems() == 0)
	{
		return;
	}

	UObject* FirstItem = TabItemListView->GetItemAt(0);
	SelectItem(FirstItem);
}

void UEditorMultiTabWidgetBase::OnTabItemSelectionChanged(UObject* InObject)
{
	SelectedIndex = TabItemListView->GetIndexForItem(InObject);
	if (SelectedIndex >= 0)
	{
		if (TabWidgetSwitcher)
		{
			TabWidgetSwitcher->SetActiveWidgetIndex(SelectedIndex);
		}

		OnSelectedItemChanged(InObject);
	}
}
