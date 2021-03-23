// Dog Fight Game Code By CYM.


#include "CardDisplayWidget.h"


#include "Chaos/AABB.h"
#include "Chaos/AABB.h"

UCardDisplayWidget::UCardDisplayWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCardItemSelectable = true;
}


void UCardDisplayWidget::SetCardDisplayInfos(const TArray<FCardInstanceDisplayInfo>& CardDisplayInfos)
{
	CardDisplayInfoList = CardDisplayInfos;

	// Invoke the Blueprint implementation
	OnCardInfoListChanged();
}

void UCardDisplayWidget::SelectCardByIndex(int32 Index)
{
	if (!SelectedCardIndexList.Contains(Index))
	{
		SelectedCardIndexList.Add(Index);

		HandleSelectionChanged();
	}
}

void UCardDisplayWidget::DeselectCardByIndex(int32 Index)
{
	if (SelectedCardIndexList.Contains(Index))
	{
		SelectedCardIndexList.Remove(Index);

		HandleSelectionChanged();
	}
}

void UCardDisplayWidget::ConfirmSelection()
{
	FString IndexArray;
	for(int32 Index : SelectedCardIndexList)
	{
		IndexArray.AppendInt(Index);
		IndexArray.AppendChar(' ');
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Selection: %s"), *IndexArray));

	// Disable all items after confirm
	SetCardItemsSelectable(false);

	OnCardSelectionConfirmed.Broadcast(SelectedCardIndexList);

	// Clear selected list
	SelectedCardIndexList.Empty();
}

void UCardDisplayWidget::SetSelectMode(ECardSelectionMode NewMode)
{
	if (NewMode != CardSelectionMode)
	{
		CardSelectionMode = NewMode;

		OnCardSelectionModeChanged(NewMode);
	}
}

void UCardDisplayWidget::SetCardItemsSelectable(bool bSelectable)
{
	if (bCardItemSelectable != bSelectable)
	{
		bCardItemSelectable = bSelectable;
		OnCardItemsActiveChanged(bSelectable);
	}
}

void UCardDisplayWidget::SetDesireSelectCount(int32 DesireCount)
{
	DesiredSelectCount = DesireCount;
}

void UCardDisplayWidget::HandleSelectionChanged()
{
	switch(CardSelectionMode)
	{
	case ECardSelectionMode::CSM_None:
		break;
	case ECardSelectionMode::CSM_SingleNoConfirm:
		// Automatically confirm once the selected item count meet the requirement
		if (SelectedCardIndexList.Num() >= 1)
		{
			ConfirmSelection();
		}
		break;
	case ECardSelectionMode::CSM_MultiWithConfirm:
	case ECardSelectionMode::CSM_AnyWithConfirm:
		// Trigger BP function
		OnSelectedCountChanged(SelectedCardIndexList.Num());
		break;
	default: ;
	}
}
