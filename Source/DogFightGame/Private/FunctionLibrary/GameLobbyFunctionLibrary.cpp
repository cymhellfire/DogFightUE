// Dog Fight Game Code By CYM.


#include "FunctionLibrary/GameLobbyFunctionLibrary.h"
#include "GameMode/GameLobbyGameState.h"
#include "Player/GameLobbyPlayerState.h"
#include "PlayerController/GameLobbyPlayerController.h"

TArray<AGameLobbyPlayerState*> UGameLobbyFunctionLibrary::GetAllGameLobbyPlayerState(const UObject* WorldContext)
{
	if (auto GS = GetCurrentLobbyGameState(WorldContext))
	{
		return GS->GetAllPlayerState();
	}

	return TArray<AGameLobbyPlayerState*>();
}

AGameLobbyPlayerController* UGameLobbyFunctionLibrary::GetLocalGameLobbyPlayerController(const UObject* WorldContext)
{
	if (auto World = WorldContext->GetWorld())
	{
		return Cast<AGameLobbyPlayerController>(GEngine->GetFirstLocalPlayerController(World));
	}

	return nullptr;
}

AGameLobbyPlayerState* UGameLobbyFunctionLibrary::GetLocalGameLobbyPlayerState(const UObject* WorldContext)
{
	if (auto PC = GetLocalGameLobbyPlayerController(WorldContext))
	{
		return PC->GetPlayerState<AGameLobbyPlayerState>();
	}

	return nullptr;
}

AGameLobbyGameState* UGameLobbyFunctionLibrary::GetCurrentLobbyGameState(const UObject* WorldContext)
{
	if (!IsValid(WorldContext))
		return nullptr;

	if (auto CurWorld = WorldContext->GetWorld())
	{
		return Cast<AGameLobbyGameState>(CurWorld->GetGameState());
	}

	return nullptr;
}
