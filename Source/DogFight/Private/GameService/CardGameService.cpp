#include "GameService/CardGameService.h"

void UCardGameService::UseCard(FString CardName, UObject* Instigator)
{
	UCard* NewCard = CreateCard(CardName, Instigator);
	if (NewCard)
	{
		APlayerController* Owner = Cast<APlayerController>(Instigator);
		if (Owner)
		{
			NewCard->SetOwnerController(Owner);
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
}
