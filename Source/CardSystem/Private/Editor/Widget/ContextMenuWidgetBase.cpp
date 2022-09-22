#include "Editor/Widget/ContextMenuWidgetBase.h"

#include "Components/ListView.h"
#include "Editor/ContextMenuItemData.h"

bool UContextMenuWidgetBase::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (ContextMenuItemListView)
	{
		MenuItemClickedHandle = ContextMenuItemListView->OnItemClicked().AddUObject(this, &UContextMenuWidgetBase::OnMenuItemClicked);
	}

	return true;
}

void UContextMenuWidgetBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (ContextMenuItemListView)
	{
		ContextMenuItemListView->OnItemClicked().Remove(MenuItemClickedHandle);
	}

	// Free all item data
	ClearAllItems();

	OnContextMenuItemClicked.Clear();
}

void UContextMenuWidgetBase::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	// Broadcast
	OnContextMenuFocusLost.Broadcast();
}

void UContextMenuWidgetBase::InitializeItemList(const TArray<UContextMenuItemData*>& InItemList)
{
	if (ContextMenuItemListView)
	{
		// Clear all items before add new one
		ClearAllItems();

		for (auto InItem : InItemList)
		{
			ContextMenuItemListView->AddItem(InItem);
		}

		MenuItemDataList = InItemList;
	}
}

void UContextMenuWidgetBase::OnMenuItemClicked(UObject* Item)
{
	// Trigger the item callback
	if (UContextMenuItemData* MenuItemData = Cast<UContextMenuItemData>(Item))
	{
		MenuItemData->ItemCallback();
	}

	OnContextMenuItemClicked.Broadcast();
}

void UContextMenuWidgetBase::ClearAllItems()
{
	if (ContextMenuItemListView && ContextMenuItemListView->GetNumItems() > 0)
	{
		ContextMenuItemListView->ClearListItems();
	}

	if (MenuItemDataList.Num() > 0)
	{
		for (auto ItemData : MenuItemDataList)
		{
			ItemData->ConditionalBeginDestroy();
		}
		MenuItemDataList.Empty();
	}
}
