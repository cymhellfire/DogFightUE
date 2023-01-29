#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"

#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"
#include "GameMode/TopDownStyleGameState.h"
#include "Player/TopDownStylePlayerState.h"
#include "PlayerController/TopDownStylePlayerController.h"
#include "PlayerController/PlayerControllerComponent/InGameWidgetManipulatorComponent.h"

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

int32 UCommonGameplayFunctionLibrary::GetPlayerCardNums(int32 InPlayerId)
{
	if (auto PS = Cast<ATopDownStylePlayerState>(GetPlayerStateById(InPlayerId)))
	{
		return PS->GetCardNum();
	}

	return -1;
}

void UCommonGameplayFunctionLibrary::AddWidgetByPlayerId(FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo([WidgetName](ATopDownStylePlayerController* InPC)
		{
			if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
			{
				Manipulator->ClientAddInGameWidget(WidgetName);
			}
		}, InPlayerId);
}

void UCommonGameplayFunctionLibrary::ShowWidgetByPlayerId(FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo([WidgetName](ATopDownStylePlayerController* InPC)
		{
			if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
			{
				Manipulator->ClientShowInGameWidget(WidgetName);
			}
		}, InPlayerId);
}

void UCommonGameplayFunctionLibrary::HideWidgetbyPlayerId(FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo([WidgetName](ATopDownStylePlayerController* InPC)
		{
			if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
			{
				Manipulator->ClientShowInGameWidget(WidgetName);
			}
		}, InPlayerId);
}

void UCommonGameplayFunctionLibrary::RemoveWidgetPlayerId(FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo([WidgetName](ATopDownStylePlayerController* InPC)
		{
			if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
			{
				Manipulator->ClientRemoveInGameWidget(WidgetName);
			}
		}, InPlayerId);
}

int32 UCommonGameplayFunctionLibrary::GetAlivePlayerNum()
{
	int32 Result = 0;
	ForEachPlayerStateDo([&Result](ATopDownStylePlayerState* PS)
	{
		if (PS->CurrentState == ETopDownStylePlayerState::PS_Alive)
		{
			Result++;
		}
	});

	return Result;
}

void UCommonGameplayFunctionLibrary::ForEachPlayerStateDo(TFunction<void(ATopDownStylePlayerState*)> ExecuteFunc,
	int32 PlayerIdMask)
{
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		for (auto PS : GameState->PlayerArray)
		{
			if (!PS)
				continue;

			// Check the player id mask
			if (PlayerIdMask != -1 && PlayerIdMask != PS->GetPlayerId())
			{
				continue;
			}

			if (auto CastPS = Cast<ATopDownStylePlayerState>(PS))
			{
				ExecuteFunc(CastPS);
			}
		}
	}
}

void UCommonGameplayFunctionLibrary::ForEachPlayerControllerDo(TFunction<void(ATopDownStylePlayerController*)> ExecuteFunc
	, int32 PlayerIdMask)
{
	if (auto GameState = GetCurrentTopDownStyleGameState())
	{
		for (auto PS : GameState->PlayerArray)
		{
			if (!PS)
				continue;

			// Check the player id mask
			if (PlayerIdMask != -1 && PlayerIdMask != PS->GetPlayerId())
			{
				continue;
			}

			if (auto PC = Cast<ATopDownStylePlayerController>(PS->GetPlayerController()))
			{
				// Execute the function
				ExecuteFunc(PC);
			}
		}
	}
}
