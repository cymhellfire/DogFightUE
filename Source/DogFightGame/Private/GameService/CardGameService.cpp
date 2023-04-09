#include "GameService/CardGameService.h"

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
