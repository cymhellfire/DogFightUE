#include "Editor/Widget/CardEditorTabItemWidget.h"

#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Editor/CardEditor.h"
#include "Editor/CardVisual.h"
#include "Editor/EditingCard.h"
#include "Editor/Widget/EditorTextInputPopupWidget.h"
#include "GameService/CardEditorService.h"

bool UCardEditorTabItemWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (CloseTabButton)
	{
		CloseTabButton->OnClicked.AddDynamic(this, &UCardEditorTabItemWidget::OnCloseTabButtonClicked);
	}
	return true;
}

void UCardEditorTabItemWidget::BeginDestroy()
{
	if (BoundCard.IsValid())
	{
		ClearCardListener(BoundCard.Pin().Get());
	}
	if (CloseTabButton)
	{
		CloseTabButton->OnClicked.RemoveDynamic(this, &UCardEditorTabItemWidget::OnCloseTabButtonClicked);
	}

	Super::BeginDestroy();
}

void UCardEditorTabItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	if (UCardVisual* CardVisual = Cast<UCardVisual>(ListItemObject))
	{
		InitializeWithCard(CardVisual->GetCard(), CardVisual->GetFocusId());
	}
}

void UCardEditorTabItemWidget::InitializeWithCard(TWeakPtr<FEditingCard> InCard, int32 FocusId)
{
	if (!InCard.IsValid() || BoundCard == InCard)
	{
		return;
	}

	if (BoundCard.IsValid())
	{
		ClearCardListener(BoundCard.Pin().Get());
	}

	BindCardListener(InCard.Pin().Get());
	BoundCard = InCard;
	BoundId = FocusId;

	if (CardNameTextBlock)
	{
		CardNameTextBlock->SetText(FText::FromString(InCard.Pin()->GetCardName()));
	}
}

void UCardEditorTabItemWidget::BindCardListener(FEditingCard* InCard)
{
	CardNameChangedHandle = InCard->OnEditingCardNameChanged.AddUObject(this, &UCardEditorTabItemWidget::OnCardNameChanged);
}

void UCardEditorTabItemWidget::ClearCardListener(FEditingCard* InCard)
{
	InCard->OnEditingCardNameChanged.Remove(CardNameChangedHandle);
}

void UCardEditorTabItemWidget::OnCardNameChanged(FString InName)
{
	if (CardNameTextBlock)
	{
		CardNameTextBlock->SetText(FText::FromString(InName));
	}
}

void UCardEditorTabItemWidget::OnCloseTabButtonClicked()
{
	UCardEditorService* CardEditorService = UGameService::GetGameServiceBySuperClass<UCardEditorService>();
	if (CardEditorService)
	{
		auto CardEditor = CardEditorService->GetCardEditor();
		if (CardEditor.IsValid())
		{
			auto PinnedCardEditor = CardEditor.Pin();
			PinnedCardEditor->CloseCard(BoundId);
		}
	}
}
