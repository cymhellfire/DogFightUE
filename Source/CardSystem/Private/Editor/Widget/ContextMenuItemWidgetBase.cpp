#include "Editor/Widget/ContextMenuItemWidgetBase.h"

#include "Components/RichTextBlock.h"
#include "Editor/ContextMenuItemData.h"

void UContextMenuItemWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UContextMenuItemData* ContextMenuItemData = Cast<UContextMenuItemData>(ListItemObject))
	{
		InitializeWithItemData(ContextMenuItemData);
	}
}

void UContextMenuItemWidgetBase::InitializeWithItemData(UContextMenuItemData* InItemData)
{
	if (InItemData == nullptr || InItemData == ItemData)
	{
		return;
	}

	if (ItemNameTextBlock)
	{
		ItemNameTextBlock->SetText(InItemData->ItemName);
	}

	ItemData = InItemData;
}
