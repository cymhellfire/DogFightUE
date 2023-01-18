#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"

#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
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
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		if (auto Timeline = GameMode->GetGameTimelineComponent())
		{
			Timeline->InitializeTimeline();
		}
	}
}

TArray<int32> UCommonGameFlowFunctionLibrary::GetCurrentTimeline()
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		if (auto Timeline = GameMode->GetGameTimelineComponent())
		{
			return Timeline->GetTimeline();
		}
	}

	return TArray<int32>();
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

ATopDownStyleGameMode* UCommonGameFlowFunctionLibrary::GetCurrentTopDownStyleGameMode()
{
	if (auto CurWorld = ULuaIntegrationFunctionLibrary::GetCurrentWorld())
	{
		if (auto GameMode = Cast<ATopDownStyleGameMode>(CurWorld->GetAuthGameMode()))
		{
			return GameMode;
		}
	}

	return nullptr;
}

ATopDownStyleGameState* UCommonGameFlowFunctionLibrary::GetCurrentTopDownStyleGameState()
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		return GameMode->GetGameState<ATopDownStyleGameState>();
	}

	return nullptr;
}
