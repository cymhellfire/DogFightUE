// Dog Fight Game Code By CYM.


#include "CardDisplayWidget.h"


#include "Chaos/AABB.h"
#include "Chaos/AABB.h"

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
	
	OnCardSelectionConfirmed.Broadcast(SelectedCardIndexList);
}

void UCardDisplayWidget::SetSelectMode(ECardSelectionMode NewMode)
{
	if (NewMode != CardSelectionMode)
	{
		CardSelectionMode = NewMode;

		OnCardSelectionModeChanged(NewMode);
	}
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
		break;
	default: ;
	}
}
