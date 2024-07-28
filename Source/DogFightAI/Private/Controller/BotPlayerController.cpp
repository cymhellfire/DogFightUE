// Dog Fight Game Code By CYM.


#include "Controller/BotPlayerController.h"

#include "GameFramework/PlayerState.h"

ABotPlayerController::ABotPlayerController()
{
	// Bot player also need player state
	bWantsPlayerState = true;
}

void ABotPlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	
}

void ABotPlayerController::SetPlayerId(int32 InId)
{
	if (auto PS = GetPlayerState<APlayerState>())
	{
		PS->SetPlayerId(InId);
	}
}
