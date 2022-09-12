#include "Editor/Widget/CardFunctionTabWidget.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "Editor/CardCommandEditingData.h"
#include "Editor/EditingCardFunction.h"
#include "Editor/Widget/CommandListPopupWidget.h"
#include "GameService/CardEditorService.h"

bool UCardFunctionTabWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (AddCommandButton)
	{
		AddCommandButton->OnClicked.AddDynamic(this, &UCardFunctionTabWidget::OnAddCommandButtonClicked);
	}

	return true;
}

void UCardFunctionTabWidget::BeginDestroy()
{
	Super::BeginDestroy();

	if (AddCommandButton)
	{
		AddCommandButton->OnClicked.RemoveDynamic(this, &UCardFunctionTabWidget::OnAddCommandButtonClicked);
	}
}

void UCardFunctionTabWidget::InitializeWithFunction(TWeakPtr<FEditingCardFunction> InFunction)
{
	if (!InFunction.IsValid() || EditingFunction == InFunction)
	{
		return;
	}

	if (EditingFunction.IsValid())
	{
		ClearFunctionListener(EditingFunction.Pin().Get());
	}

	EditingFunction = InFunction;
	BindFunctionListener(EditingFunction.Pin().Get());
}

void UCardFunctionTabWidget::BindFunctionListener(FEditingCardFunction* InFunction)
{
	FunctionCommandAddedHandle = InFunction->OnFunctionCommandAdded.AddUObject(this, &UCardFunctionTabWidget::OnFunctionCommandAdded);
}

void UCardFunctionTabWidget::ClearFunctionListener(FEditingCardFunction* InFunction)
{
	InFunction->OnFunctionCommandAdded.Remove(FunctionCommandAddedHandle);
}

void UCardFunctionTabWidget::OnCommandIdConfirmed(int32 CmdId)
{
	if (!EditingFunction.IsValid() || CmdId < 0)
	{
		return;
	}

	auto PinnedFunction = EditingFunction.Pin();
	PinnedFunction->AddCommand(CmdId);
}

void UCardFunctionTabWidget::OnFunctionCommandAdded(TWeakPtr<FEditingCardCommand> InCommand)
{
	if (!InCommand.IsValid())
	{
		return;
	}

	UCardCommandEditingData* EditingData = NewObject<UCardCommandEditingData>(this);
	EditingData->EditingCommand = InCommand;

	CommandEditingDataList.AddUnique(EditingData);

	if (FunctionCommandListView)
	{
		FunctionCommandListView->AddItem(EditingData);
	}
}

void UCardFunctionTabWidget::OnAddCommandButtonClicked()
{
	UCardEditorService* EditorService = UGameService::GetGameServiceBySuperClass<UCardEditorService>();
	if (EditorService)
	{
		auto Popup = EditorService->OpenCommandListPopupWidget();
		Popup->OnCommandItemConfirm.AddUObject(this, &UCardFunctionTabWidget::OnCommandIdConfirmed);
	}
}
