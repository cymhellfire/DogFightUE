#include "Editor/Widget/EditorSelectableListItemBase.h"
#include "Components/Border.h"

void UEditorSelectableListItemBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (ItemBorder)
	{
		ItemBorder->SetBrush(bIsSelected ? SelectedBrush : DefaultBrush);
	}
}
