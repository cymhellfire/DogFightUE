#pragma once

#include "CoreMinimal.h"
#include "CardVisual.generated.h"

class FEditingCard;

UCLASS(MinimalAPI)
class UCardVisual : public UObject
{
	GENERATED_BODY()
public:
	UCardVisual(){}

	void SetFocusId(int32 InId)
	{
		FocusId = InId;
	}

	void SetEditingCard(TWeakPtr<FEditingCard> InCard)
	{
		EditingCard = InCard;
	}

	int32 GetFocusId() const
	{
		return FocusId;
	}

	TWeakPtr<FEditingCard> GetCard() const
	{
		return EditingCard;
	}

private:
	int32 FocusId;
	TWeakPtr<FEditingCard> EditingCard;
};
