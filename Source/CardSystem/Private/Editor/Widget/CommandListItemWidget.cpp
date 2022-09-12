#include "Editor/Widget/CommandListItemWidget.h"

#include "CardSystem.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"
#include "Editor/CardCommandVisual.h"

UCommandListItemWidget::UCommandListItemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UCommandListItemWidget::InitializeWithCommand(UCardCommandVisual* InCommand)
{
	if (InCommand == nullptr)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[CardCommandItemWidget] Invalid command to initialized with."));
		return;
	}

	// Initialize interface
	CommandNameTextBlock->SetText(InCommand->GetCommandName());
	CommandIconSwitcher->SetActiveWidgetIndex(InCommand->GetCommandType());
}

void UCommandListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UCardCommandVisual* CommandVisual = Cast<UCardCommandVisual>(ListItemObject);
	if (CommandVisual == nullptr)
	{
		return;
	}

	InitializeWithCommand(CommandVisual);
}
