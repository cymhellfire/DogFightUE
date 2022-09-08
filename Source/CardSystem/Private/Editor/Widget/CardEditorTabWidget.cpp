#include "Editor/Widget/CardEditorTabWidget.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/RichTextBlock.h"
#include "Editor/CardFunctionVisual.h"
#include "Editor/EditingCard.h"
#include "Editor/EditingCardFunction.h"
#include "Editor/Widget/EditorTextInputPopupWidget.h"
#include "GameService/CardEditorService.h"

bool UCardEditorTabWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (CardRenameButton)
	{
		CardRenameButton->OnClicked.AddDynamic(this, &UCardEditorTabWidget::OnCardRenameButtonClicked);
	}
	if (CreateNewCardFunctionButton)
	{
		CreateNewCardFunctionButton->OnClicked.AddDynamic(this, &UCardEditorTabWidget::OnCreateNewCardFunctionButtonClicked);
	}

	return true;
}

void UCardEditorTabWidget::BeginDestroy()
{
	Super::BeginDestroy();

	if (CardRenameButton)
	{
		CardRenameButton->OnClicked.RemoveDynamic(this, &UCardEditorTabWidget::OnCardRenameButtonClicked);
	}
	if (CreateNewCardFunctionButton)
	{
		CreateNewCardFunctionButton->OnClicked.RemoveDynamic(this, &UCardEditorTabWidget::OnCreateNewCardFunctionButtonClicked);
	}
}

void UCardEditorTabWidget::BindEditingCard(TWeakPtr<FEditingCard> InCard)
{
	if (!InCard.IsValid() || InCard == EditingCard)
	{
		return;
	}

	BindCardListener(InCard);
	EditingCard = InCard;

	if (CardNameTextBlock)
	{
		CardNameTextBlock->SetText(FText::FromString(InCard.Pin()->GetCardName()));
	}
}

void UCardEditorTabWidget::BindCardListener(TWeakPtr<FEditingCard> InCard)
{
	auto PinnedCard = InCard.Pin();
	CardNameChangedHandle = PinnedCard->OnEditingCardNameChanged.AddUObject(this, &UCardEditorTabWidget::OnCardNameChanged);
	CardFunctionAddedHandle = PinnedCard->OnEditingCardFunctionAdded.AddUObject(this, &UCardEditorTabWidget::OnCardFunctionAdded);
}

void UCardEditorTabWidget::ClearCardListener(TWeakPtr<FEditingCard> InCard)
{
	auto PinnedCard = InCard.Pin();
	PinnedCard->OnEditingCardNameChanged.Remove(CardNameChangedHandle);
	PinnedCard->OnEditingCardFunctionAdded.Remove(CardFunctionAddedHandle);
}

void UCardEditorTabWidget::ClearCardFunctions()
{
	if (CardFunctionsListView)
	{
		CardFunctionsListView->ClearListItems();
	}

	for (auto FunctionVisual : CardFunctionVisualList)
	{
		FunctionVisual->ConditionalBeginDestroy();
	}
	CardFunctionVisualList.Empty();
}

void UCardEditorTabWidget::OnCardNameChanged(FString InName)
{
	if (CardNameTextBlock)
	{
		CardNameTextBlock->SetText(FText::FromString(InName));
	}
}

void UCardEditorTabWidget::OnCardFunctionAdded(TWeakPtr<FEditingCardFunction> InFunction)
{
	if (!InFunction.IsValid() || CardFunctionsListView == nullptr)
	{
		return;
	}

	auto PinnedFunction = InFunction.Pin();
	UCardFunctionVisual* NewFunctionVisual = NewObject<UCardFunctionVisual>();
	NewFunctionVisual->Initialize(PinnedFunction->GetID(), PinnedFunction->GetName(), InFunction);

	CardFunctionVisualList.Add(NewFunctionVisual);
	CardFunctionsListView->AddItem(NewFunctionVisual);

	// Add new function tab
	AddNewTab(NewFunctionVisual);
}

void UCardEditorTabWidget::OnCardRenameButtonClicked()
{
	UCardEditorService* EditorService = UGameService::GetGameServiceBySuperClass<UCardEditorService>();
	if (EditorService)
	{
		FTextInputPopupParameter NewParam;
		NewParam.Title = FText::FromString("Rename");
		NewParam.PopupSize = FVector2D(650, 250);
		NewParam.HintText = FText::FromString("Type new name");
		auto Popup = EditorService->OpenTextInputPopupWidget(NewParam);
		if (Popup)
		{
			Popup->OnInputTextConfirmed.AddUObject(this, &UCardEditorTabWidget::OnCardRenamePopupConfirmed);
		}
	}
}

void UCardEditorTabWidget::OnCardRenamePopupConfirmed(FText InText)
{
	if (EditingCard.IsValid())
	{
		auto PinnedCard = EditingCard.Pin();
		PinnedCard->SetCardName(InText.ToString());
	}
}

void UCardEditorTabWidget::OnCreateNewCardFunctionButtonClicked()
{
	UCardEditorService* EditorService = UGameService::GetGameServiceBySuperClass<UCardEditorService>();
	if (EditorService)
	{
		FTextInputPopupParameter NewParam;
		NewParam.Title = FText::FromString("Function Name");
		NewParam.PopupSize = FVector2D(650, 250);
		NewParam.HintText = FText::FromString("Type function name");
		auto Popup = EditorService->OpenTextInputPopupWidget(NewParam);
		if (Popup)
		{
			Popup->OnInputTextConfirmed.AddUObject(this, &UCardEditorTabWidget::OnCreateNewCardFunctionPopupConfirmed);
		}
	}
}

void UCardEditorTabWidget::OnCreateNewCardFunctionPopupConfirmed(FText InText)
{
	if (EditingCard.IsValid() && !InText.IsEmpty())
	{
		auto PinnedCard = EditingCard.Pin();

		PinnedCard->CreateNewFunction(InText.ToString());
	}
}
