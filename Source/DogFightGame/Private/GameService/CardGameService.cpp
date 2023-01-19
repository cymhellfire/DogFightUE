#include "GameService/CardGameService.h"

void UCardGameService::UseCard(FString CardName, UObject* Instigator)
{
	UCard* NewCard = CreateCard(CardName, Instigator);
	if (NewCard)
	{
		HoldingCard = NewCard;
		// APlayerController* Owner = Cast<APlayerController>(Instigator);
		// if (Owner)
		// {
		// 	NewCard->SetOwnerController(Owner);
		//
		// 	if (auto PlayerState = Owner->GetPlayerState<AStandardPlayerState>())
		// 	{
		// 		PlayerState->AddCardDescObject(NewCard->GetDescObject());
		// 	}
		// }
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
	// if (APlayerController* PC = Cast<APlayerController>(Card->GetOwnerController()))
	// {
	// 	if (auto PlayerState = PC->GetPlayerState<AStandardPlayerState>())
	// 	{
	// 		PlayerState->RemoveCardDescObject(Card->GetDescObject());
	// 	}
	// }

	DestroyCard(Card);

	HoldingCard = nullptr;
}
