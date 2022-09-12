#include "Editor/Widget/EditorSelectableListItemBase.h"
#include "Components/Border.h"

bool UEditorSelectableListItemBase::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (ItemBorder)
	{
		ItemBorder->SetBrush(DefaultBrush);
	}

	return true;
}

void UEditorSelectableListItemBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (ItemBorder)
	{
		ItemBorder->SetBrush(bIsSelected ? SelectedBrush : DefaultBrush);
	}
}
