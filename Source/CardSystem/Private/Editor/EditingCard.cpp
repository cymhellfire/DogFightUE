#include "Editor/EditingCard.h"
#include "Editor/EditingCardFunction.h"
#include "FunctionLibrary/MathHelperLibrary.h"

FEditingCard::FEditingCard()
{
	CardName = "New Card";
}

void FEditingCard::SetCardName(FString InName)
{
	if (CardName == InName)
		return;

	CardName = InName;
	OnEditingCardNameChanged.Broadcast(CardName);
}

void FEditingCard::CreateNewFunction(const FString& InName)
{
	// Get available ID
	TArray<int32> IDList;
	FunctionMap.GenerateKeyArray(IDList);
	int32 AvailableID = UMathHelperLibrary::GetMinMissingPositiveNumber(IDList, 0);

	TSharedPtr<FEditingCardFunction> NewFunction = MakeShareable(new FEditingCardFunction);
	NewFunction->SetID(AvailableID);
	NewFunction->SetName(InName);

	FunctionMap.Add(AvailableID,NewFunction);
	OnEditingCardFunctionAdded.Broadcast(NewFunction);
}
