#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"

#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/TopDownStyleGameState.h"
#include "GameMode/GameStateComponent/GameTimelineComponent.h"
#include "PlayerController/TopDownStylePlayerController.h"

TArray<ATopDownStylePlayerController*> UCommonGameFlowFunctionLibrary::GetAllPlayerControllers(const UObject* WorldContextObject)
{
	TArray<ATopDownStylePlayerController*> Result;
	if (auto GameMode = GetCurrentTopDownStyleGameMode(WorldContextObject))
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

FName UCommonGameFlowFunctionLibrary::GetCurrentGameFlowStateName(const UObject* WorldContextObject)
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode(WorldContextObject))
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

void UCommonGameFlowFunctionLibrary::SetCharacterMoveEnableForAllPlayers(const UObject* WorldContextObject, bool bEnable)
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode(WorldContextObject))
	{
		GameMode->SetEnableCharacterMoveForAllPlayers(bEnable);
	}
}

void UCommonGameFlowFunctionLibrary::InitializeGameTimeline(const UObject* WorldContextObject)
{
	// Use game mode to get timeline component to ensure this operation cannot be finished on client side.
	if (auto Timeline = GetCurrentTimeline_Server(WorldContextObject))
	{
		Timeline->InitializeTimeline();
	}
}

TArray<int32> UCommonGameFlowFunctionLibrary::GetCurrentTimeline(const UObject* WorldContextObject)
{
	if (auto Timeline = GetCurrentTimeline_Common(WorldContextObject))
	{
		return Timeline->GetTimeline();
	}

	return TArray<int32>();
}

void UCommonGameFlowFunctionLibrary::MoveTimelineForward(const UObject* WorldContextObject)
{
	if (auto Timeline = GetCurrentTimeline_Server(WorldContextObject))
	{
		Timeline->MoveForward();
	}
}

int32 UCommonGameFlowFunctionLibrary::GetTimelineFirstPlayerId(const UObject* WorldContextObject)
{
	if (auto Timeline = GetCurrentTimeline_Server(WorldContextObject))
	{
		return Timeline->GetFirstPlayerId();
	}

	return -1;
}

UGameTimelineComponent* UCommonGameFlowFunctionLibrary::GetCurrentTimeline_Server(const UObject* WorldContextObject)
{
	// Only server can access the game mode.
	if (auto GameMode = GetCurrentTopDownStyleGameMode(WorldContextObject))
	{
		return GameMode->GetGameTimelineComponent();
	}
	return nullptr;
}

UGameTimelineComponent* UCommonGameFlowFunctionLibrary::GetCurrentTimeline_Common(const UObject* WorldContextObject)
{
	// Game state is available both on server and client.
	if (auto GameState = GetCurrentTopDownStyleGameState(WorldContextObject))
	{
		return GameState->GetGameTimelineComponent();
	}
	return nullptr;
}

int32 UCommonGameFlowFunctionLibrary::GetCurrentPlayerId(const UObject* WorldContextObject)
{
	if (auto GameState = GetCurrentTopDownStyleGameState(WorldContextObject))
	{
		return GameState->GetCurrentPlayerId();
	}
	return -1;
}

void UCommonGameFlowFunctionLibrary::SetCurrentPlayerId(const UObject* WorldContextObject, int32 InId)
{
	if (auto GameState = GetCurrentTopDownStyleGameState(WorldContextObject))
	{
		GameState->SetCurrentPlayerId(InId);
	}
}

void UCommonGameFlowFunctionLibrary::SyncCurrentPlayerIdWithTimeline(const UObject* WorldContextObject)
{
	if (auto GameState = GetCurrentTopDownStyleGameState(WorldContextObject))
	{
		if (auto Timeline = GameState->GetGameTimelineComponent())
		{
			GameState->SetCurrentPlayerId(Timeline->GetFirstPlayerId());
		}
	}
}

void UCommonGameFlowFunctionLibrary::RequestFinishLocalPlayerRound(const UObject* WorldContextObject)
{
	if (auto PlayerController = GetLocalPlayerController(WorldContextObject))
	{
		PlayerController->ServerRequestFinishRound();
	}
}

void UCommonGameFlowFunctionLibrary::BroadcastStartPlayerRound(const UObject* WorldContextObject, int32 PlayerId)
{
	UCommonGameplayFunctionLibrary::ForEachPlayerControllerDo(WorldContextObject, [PlayerId](ATopDownStylePlayerController* PlayerController)
	{
		PlayerController->ClientSendPlayerRoundStartEvent(PlayerId);
	});
}

void UCommonGameFlowFunctionLibrary::BroadcastFinishPlayerRound(const UObject* WorldContextObject, int32 PlayerId)
{
	UCommonGameplayFunctionLibrary::ForEachPlayerControllerDo(WorldContextObject, [PlayerId](ATopDownStylePlayerController* PlayerController)
	{
		PlayerController->ClientSendPlayerRoundFinishEvent(PlayerId);
	});
}

ATopDownStylePlayerController* UCommonGameFlowFunctionLibrary::GetLocalPlayerController(const UObject* WorldContextObject)
{
	if (auto World = ULuaIntegrationFunctionLibrary::GetCurrentWorld(WorldContextObject))
	{
		UE_LOG(LogTemp, Log, TEXT("Get Player Controller with id: %d"), (int32)GPlayInEditorID);
		return Cast<ATopDownStylePlayerController>(GEngine->GetFirstLocalPlayerController(World));
	}

	return nullptr;
}

ULocalPlayer* UCommonGameFlowFunctionLibrary::GetLocalPlayer(const UObject* WorldContextObject)
{
	if (auto GameInstance = ULuaIntegrationFunctionLibrary::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetLocalPlayerByIndex(0);
	}

	return nullptr;
}

int32 UCommonGameFlowFunctionLibrary::GetLocalPlayerId(const UObject* WorldContextObject)
{
	if (auto PC = GetLocalPlayerController(WorldContextObject))
	{
		if (auto PS = PC->GetPlayerState<APlayerState>())
		{
			return PS->GetPlayerId();
		}
	}

	return -1;
}
