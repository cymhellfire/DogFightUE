#include "Editor/Widget/FunctionCommandListItemWidget.h"

#include "Components/RichTextBlock.h"
#include "Editor/CardCommandEditingData.h"
#include "Editor/CardCommandVisual.h"
#include "Editor/EditingCardCommand.h"
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
