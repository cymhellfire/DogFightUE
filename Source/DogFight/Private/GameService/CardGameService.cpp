#include "GameService/CardGameService.h"

#include "Pawns/StandardModePlayerCharacter.h"

void UCardGameService::UseCard(FString CardName, UObject* Instigator, AStandardModePlayerCharacter* Carrier)
{
	UCard* NewCard = CreateCard(CardName, Instigator);
	if (NewCard)
	{
		HoldingCard = NewCard;
		APlayerController* Owner = Cast<APlayerController>(Instigator);
		if (Owner)
		{
			NewCard->SetOwnerController(Owner);
		}
		if (Carrier)
		{
			Carrier->AddCardDescObject(NewCard->GetDescObject());
		}
		NewCard->OnCardExecutionFinished.AddDynamic(this, &UCardGameService::OnCardFinished);
		NewCard->Execute();
	}
}

UClass* UCardGameService::GetDefaultCardClass() const
{
	return UCard::StaticClass();
}

void UCardGameService::OnCardFinished(ECardExecutionResult Result, UCard* Card)
{
	DestroyCard(Card);

	HoldingCard = nullptr;
}
