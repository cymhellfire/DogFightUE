#include "Editor/Widget/EditorTabItemBase.h"

#include "Components/Button.h"

bool UEditorTabItemBase::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (CloseTabButton)
	{
		CloseTabButton->OnClicked.AddDynamic(this, &UEditorTabItemBase::OnCloseTabButtonClicked);
	}

	return true;
}

void UEditorTabItemBase::BeginDestroy()
{
	if (CloseTabButton)
	{
		CloseTabButton->OnClicked.RemoveDynamic(this, &UEditorTabItemBase::OnCloseTabButtonClicked);
	}

	Super::BeginDestroy();
}

void UEditorTabItemBase::OnCloseTabButtonClicked()
{
}
