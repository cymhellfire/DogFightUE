#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"

#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/TopDownStyleGameState.h"
#include "GameMode/GameStateComponent/GameTimelineComponent.h"
#include "PlayerController/TopDownStylePlayerController.h"

TArray<ATopDownStylePlayerController*> UCommonGameFlowFunctionLibrary::GetAllPlayerControllers()
{
	TArray<ATopDownStylePlayerController*> Result;
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		auto PCList = GameMode->GetAllPlayerControllers();
		for (auto PC : PCList)
		{
			if (PC.IsValid())
			{
				Result.Add(PC.Get());
			}
		}
	}

	return Result;
}

FName UCommonGameFlowFunctionLibrary::GetCurrentGameFlowStateName()
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		return GameMode->GetCurrentGameFlowStateName();
	}

	return NAME_None;
}

void UCommonGameFlowFunctionLibrary::SpawnPlayerCharacterPawn(ATopDownStylePlayerController* Controller)
{
	if (!IsValid(Controller))
	{
		return;
	}

	Controller->SpawnCharacterPawn();
}

void UCommonGameFlowFunctionLibrary::SetCharacterMoveEnableForAllPlayers(bool bEnable)
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		GameMode->SetEnableCharacterMoveForAllPlayers(bEnable);
	}
}

void UCommonGameFlowFunctionLibrary::InitializeGameTimeline()
{
	// Use game mode to get timeline component to ensure this operation cannot be finished on client side.
	if (auto Timeline = GetCurrentTimeline_Server())
	{
		Timeline->InitializeTimeline();
	}
}

TArray<int32> UCommonGameFlowFunctionLibrary::GetCurrentTimeline()
{
	if (auto Timeline = GetCurrentTimeline_Common())
	{
		return Timeline->GetTimeline();
	}

	return TArray<int32>();
}

void UCommonGameFlowFunctionLibrary::MoveTimelineForward()
{
	if (auto Timeline = GetCurrentTimeline_Server())
	{
		Timeline->MoveForward();
	}
}

UGameTimelineComponent* UCommonGameFlowFunctionLibrary::GetCurrentTimeline_Server()
{
	// Only server can access the game mode.
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		return GameMode->GetGameTimelineComponent();
	}
	return nullptr;
}

UGameTimelineComponent* UCommonGameFlowFunctionLibrary::GetCurrentTimeline_Common()
{
	// Game state is available both on server and client.
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		return GameState->GetGameTimelineComponent();
	}
	return nullptr;
}

int32 UCommonGameFlowFunctionLibrary::GetCurrentPlayerId()
{
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		return GameState->GetCurrentPlayerId();
	}
	return -1;
}

void UCommonGameFlowFunctionLibrary::SetCurrentPlayerId(int32 InId)
{
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		GameState->SetCurrentPlayerId(InId);
	}
}

void UCommonGameFlowFunctionLibrary::SyncCurrentPlayerIdWithTimeline()
{
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		if (auto Timeline = GameState->GetGameTimelineComponent())
		{
			GameState->SetCurrentPlayerId(Timeline->GetFirstPlayerId());
		}
	}
}

ATopDownStylePlayerController* UCommonGameFlowFunctionLibrary::GetLocalPlayerController()
{
	if (auto World = ULuaIntegrationFunctionLibrary::GetCurrentWorld())
	{
		return Cast<ATopDownStylePlayerController>(GEngine->GetFirstLocalPlayerController(World));
	}

	return nullptr;
}

ULocalPlayer* UCommonGameFlowFunctionLibrary::GetLocalPlayer()
{
	if (auto GameInstance = ULuaIntegrationFunctionLibrary::GetGameInstance())
	{
		return GameInstance->GetLocalPlayerByIndex(0);
	}

	return nullptr;
}

int32 UCommonGameFlowFunctionLibrary::GetLocalPlayerId()
{
	if (auto PC = GetLocalPlayerController())
	{
		if (auto PS = PC->GetPlayerState<APlayerState>())
		{
			return PS->GetPlayerId();
		}
	}

	return -1;
}
