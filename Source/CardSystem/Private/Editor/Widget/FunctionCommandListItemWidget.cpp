#include "Editor/Widget/FunctionCommandListItemWidget.h"

#include "Components/RichTextBlock.h"
#include "Editor/CardCommandEditingData.h"
#include "Editor/CardCommandVisual.h"
#include "Editor/ContextMenuItemData.h"
#include "Editor/EditingCardCommand.h"
#include "Editor/Widget/ContextMenuWidgetBase.h"
#include "GameService/CommandManagerService.h"

void UFunctionCommandListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	if (UCardCommandEditingData* EditingData = Cast<UCardCommandEditingData>(ListItemObject))
	{
		InitializeWithEditingCommand(EditingData);
	}
}

void UFunctionCommandListItemWidget::InitializeWithEditingCommand(UCardCommandEditingData* InData)
{
	auto InEditingCommand = InData->EditingCommand;
	if (!InEditingCommand.IsValid() || InEditingCommand == EditingCommand)
	{
		return;
	}

	if (EditingCommand.IsValid())
	{
		ClearCommandListener(EditingCommand.Pin().Get());
	}

	auto PinnedCommand = InEditingCommand.Pin();

	if (UCommandManagerService* CommandManagerService = UGameService::GetGameServiceBySuperClass<UCommandManagerService>())
	{
		auto CommandData = CommandManagerService->GetCommandVisual(PinnedCommand->GetCommandID());
		if (CommandData && CommandContextTextBlock)
		{
			CommandContextTextBlock->SetText(CommandData->GetCommandDesc());
		}
	}

	BindCommandListener(PinnedCommand.Get());
	EditingCommand = InEditingCommand;
}

void UFunctionCommandListItemWidget::BindCommandListener(FEditingCardCommand* InCommand)
{
	CommandIdChangedHandle = InCommand->OnCommandIdChanged.AddUObject(this, &UFunctionCommandListItemWidget::OnCommandIdChanged);
}

void UFunctionCommandListItemWidget::ClearCommandListener(FEditingCardCommand* InCommand)
{
	InCommand->OnCommandIdChanged.Remove(CommandIdChangedHandle);
}

void UFunctionCommandListItemWidget::OnCommandIdChanged(int32 InId)
{
	if (UCommandManagerService* CommandManagerService = UGameService::GetGameService<UCommandManagerService>())
	{
		auto CommandData = CommandManagerService->GetCommandVisual(InId);
		if (CommandData && CommandContextTextBlock)
		{
			CommandContextTextBlock->SetText(CommandData->GetCommandDesc());
		}
	}
}

void UFunctionCommandListItemWidget::OnContextMenuItemClicked()
{
	UContextMenuWidgetBase* MenuWidget = Cast<UContextMenuWidgetBase>(ContextMenuWidget);
	if (MenuWidget)
	{
		MenuWidget->OnContextMenuItemClicked.Remove(ContextMenuItemClickedHandle);
	}

	CloseContextMenu();
}

void UFunctionCommandListItemWidget::OnContextMenuOpened()
{
	if (ContextMenuWidget == nullptr)
	{
		return;
	}

	UContextMenuWidgetBase* MenuWidget = Cast<UContextMenuWidgetBase>(ContextMenuWidget);
	if (MenuWidget)
	{
		// Construct context menu options
		TArray<UContextMenuItemData*> ItemList;
		for (int32 Index = 0; Index < 2; ++Index)
		{
			UContextMenuItemData* NewOption = NewObject<UContextMenuItemData>(MenuWidget);
			NewOption->ItemName = FText::FromString(FString::Printf(TEXT("Option%d"), Index));
			NewOption->ItemCallback = [Index]
			{
				UE_LOG(LogTemp, Log, TEXT("Option%d Clicked"), Index);
			};
			ItemList.Add(NewOption);
		}

		MenuWidget->InitializeItemList(ItemList);
		if (!MenuWidget->OnContextMenuItemClicked.IsBoundToObject(this))
		{
			ContextMenuItemClickedHandle = MenuWidget->OnContextMenuItemClicked.AddUObject(this, &UFunctionCommandListItemWidget::OnContextMenuItemClicked);
		}
	}
}
