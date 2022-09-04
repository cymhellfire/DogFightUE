#pragma once

#include "CoreMinimal.h"

class FEditingCard;
class FEditingCardFunction;

class FCardEditor
{
public:
	FCardEditor();

	void CreateNewCard();

	void CloseCard(int32 InId);

	TWeakPtr<FEditingCard> GetEditingCard()
	{
		auto FoundCard = EditingCardMap.Find(FocusId);

		return FoundCard != nullptr ? *FoundCard : nullptr;
	}

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FNewCardCreatedSignature, TWeakPtr<FEditingCard>, int32)
	FNewCardCreatedSignature OnNewCardCreated;

	DECLARE_MULTICAST_DELEGATE_OneParam(FEditingCardClosedSignature, int32)
	FEditingCardClosedSignature OnCardClosed;

private:
	int32 FocusId;
	TMap<int32, TSharedPtr<FEditingCard>> EditingCardMap;
};
