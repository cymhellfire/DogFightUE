#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"

#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"
#include "GameMode/TopDownStyleGameState.h"
#include "Player/TopDownStylePlayerState.h"
#include "PlayerController/TopDownStylePlayerController.h"

APlayerState* UCommonGameplayFunctionLibrary::GetPlayerStateById(int32 InPlayerId)
{
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		return GameState->GetPlayerStateById(InPlayerId);
	}
	return nullptr;
}

ATopDownStylePlayerController* UCommonGameplayFunctionLibrary::GetPlayerControllerById(int32 InPlayerId)
{
	if (auto PS = GetPlayerStateById(InPlayerId))
	{
		return Cast<ATopDownStylePlayerController>(PS->GetPlayerController());
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

void UCommonGameplayFunctionLibrary::UseCardByInstanceId(int32 InInstanceId)
{
	if (auto PC = UCommonGameFlowFunctionLibrary::GetLocalPlayerController())
	{
		// Request use card to server
		PC->ServerUseCardByInstanceId(InInstanceId);
	}
}
