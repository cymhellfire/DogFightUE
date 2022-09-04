#include "Editor/CardEditor.h"

#include "Editor/EditingCard.h"
#include "FunctionLibrary/MathHelperLibrary.h"

FCardEditor::FCardEditor()
{
}

void FCardEditor::CreateNewCard()
{
	TArray<int32> IdList;
	EditingCardMap.GenerateKeyArray(IdList);
	FocusId = UMathHelperLibrary::GetMinMissingPositiveNumber(IdList, 0);
	TSharedPtr<FEditingCard> NewCard = MakeShareable(new FEditingCard);
	EditingCardMap.Add(FocusId, NewCard);

	// Trigger event
	OnNewCardCreated.Broadcast(NewCard, FocusId);
}

void FCardEditor::CloseCard(int32 InId)
{
	if (!EditingCardMap.Contains(InId))
	{
		return;
	}

	// Broadcast event first
	OnCardClosed.Broadcast(InId);

	// Remove editing card
	EditingCardMap.Remove(InId);
}
