#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "GameMode/TopDownStyleGameState.h"
#include "Player/TopDownStylePlayerState.h"

APlayerState* UCommonGameplayFunctionLibrary::GetPlayerStateById(int32 InPlayerId)
{
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		return GameState->GetPlayerStateById(InPlayerId);
	}
	return nullptr;
}

void UCommonGameplayFunctionLibrary::DispatchCardToPlayer(int32 InPlayerId, UCard* InCard)
{
	if (auto PlayerState = Cast<ATopDownStylePlayerState>(GetPlayerStateById(InPlayerId)))
	{
		PlayerState->AddCardObject(InCard);
	}
}
