#include "Editor/Widget/CardEditorWidget.h"

#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/RichTextBlock.h"
#include "Editor/CardEditor.h"
#include "Editor/CardFunctionVisual.h"
#include "Editor/CardVisual.h"
#include "Editor/EditingCard.h"
#include "Editor/EditingCardFunction.h"
#include "Editor/Widget/CardEditorTabWidget.h"
#include "GameService/GameService.h"
#include "Editor/Widget/CommandListPopupWidget.h"
#include "Editor/Widget/EditorTextInputPopupWidget.h"
#include "GameService/CardEditorService.h"


bool UCardEditorWidget::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (CreateNewCardButton)
	{
		CreateNewCardButton->OnClicked.AddDynamic(this, &UCardEditorWidget::OnCreateNewCardButtonClicked);
	}

	return true;
}

void UCardEditorWidget::BeginDestroy()
{
	Super::BeginDestroy();

	if (CreateNewCardButton)
	{
		CreateNewCardButton->OnClicked.RemoveDynamic(this, &UCardEditorWidget::OnCreateNewCardButtonClicked);
	}
}

void UCardEditorWidget::BindCardEditor(TWeakPtr<FCardEditor> InEditor)
{
	if (CardEditor == InEditor)
		return;

	// Clear listener
	if (CardEditor.IsValid())
	{
		auto PinnedEditor = CardEditor.Pin();
		PinnedEditor->OnNewCardCreated.Remove(NewCardCreatedHandle);
		PinnedEditor->OnCardClosed.Remove(CardClosedHandle);
	}

	CardEditor = InEditor;

	// Register listeners
	if (CardEditor.IsValid())
	{
		auto PinnedEditor = CardEditor.Pin();
		NewCardCreatedHandle = PinnedEditor->OnNewCardCreated.AddUObject(this, &UCardEditorWidget::OnNewCardCreated);
		CardClosedHandle = PinnedEditor->OnCardClosed.AddUObject(this, &UCardEditorWidget::OnCardClosed);
	}
}

void UCardEditorWidget::InitializeTabWidget(UUserWidget* InWidget, UObject* InParam)
{
	Super::InitializeTabWidget(InWidget, InParam);

	UCardVisual* CardVisual = Cast<UCardVisual>(InParam);
	UCardEditorTabWidget* EditorTabWidget = Cast<UCardEditorTabWidget>(InWidget);
	if (EditorTabWidget && CardVisual)
	{
		EditorTabWidget->BindEditingCard(CardVisual->GetCard());
	}
}

void UCardEditorWidget::OnCreateNewCardButtonClicked()
{
	if (!CardEditor.IsValid())
	{
		return;
	}

	TSharedPtr<FCardEditor> PinnedEditor = CardEditor.Pin();
	PinnedEditor->CreateNewCard();
}

void UCardEditorWidget::OnNewCardCreated(TWeakPtr<FEditingCard> NewCard, int32 FocusId)
{
	if (!NewCard.IsValid())
		return;

	// Create new card item
	UCardVisual* NewCardVisual = NewObject<UCardVisual>(this);
	NewCardVisual->SetFocusId(FocusId);
	NewCardVisual->SetEditingCard(NewCard);

	CardVisualList.AddUnique(NewCardVisual);

	// Add new tab
	AddNewTab(NewCardVisual);

	
}

void UCardEditorWidget::OnCardClosed(int32 FocusId)
{
	UCardVisual* RemoveItem = nullptr;
	for (auto Item : CardVisualList)
	{
		if (Item->GetFocusId() == FocusId)
		{
			RemoveItem = Item;
			break;
		}
	}

	if (RemoveItem)
	{
		RemoveTab(RemoveItem);

		CardVisualList.Remove(RemoveItem);
	}
}
