#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"

#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameInstance/DogFightGameInstance.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/TopDownStyleGameState.h"
#include "GameService/GameEffectService.h"
#include "Pawn/PlayerPawn/TopDownStylePlayerPawn.h"
#include "Player/TopDownStylePlayerState.h"
#include "PlayerController/TopDownStylePlayerController.h"
#include "PlayerController/PlayerControllerComponent/InGameWidgetManipulatorComponent.h"
#include "Subsystem/GameplayDataSubsystem.h"

UGameplayDataSubsystem* UCommonGameplayFunctionLibrary::GetGameplayDataSubsystem(const UObject* WorldContextObject)
{
	if (auto GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		return GameInstance->GetSubsystem<UGameplayDataSubsystem>();
	}

	return nullptr;
}

APlayerState* UCommonGameplayFunctionLibrary::GetPlayerStateById(const UObject* WorldContextObject, int32 InPlayerId)
{
	if (auto GameState = GetCurrentTopDownStyleGameState(WorldContextObject))
	{
		return GameState->GetPlayerStateById(InPlayerId);
	}
	return nullptr;
}

ATopDownStylePlayerController* UCommonGameplayFunctionLibrary::GetPlayerControllerById(const UObject* WorldContextObject, int32 InPlayerId)
{
	if (auto PS = GetPlayerStateById(WorldContextObject, InPlayerId))
	{
		return Cast<ATopDownStylePlayerController>(PS->GetPlayerController());
	}

	return nullptr;
}

ATopDownStylePlayerCharacter* UCommonGameplayFunctionLibrary::GetPlayerCharacterById(const UObject* WorldContextObject,	int32 InPlayerId)
{
	if (auto PC = GetPlayerControllerById(WorldContextObject, InPlayerId))
	{
		return PC->GetCharacterPawn();
	}

	return nullptr;
}

void UCommonGameplayFunctionLibrary::DispatchCardToPlayer(const UObject* WorldContextObject, int32 InPlayerId, UCard* InCard)
{
	if (auto PlayerState = Cast<ATopDownStylePlayerState>(GetPlayerStateById(WorldContextObject, InPlayerId)))
	{
		PlayerState->AddCardObject(InCard);
	}
}

void UCommonGameplayFunctionLibrary::UseCardByInstanceId(const UObject* WorldContextObject, int32 InInstanceId)
{
	if (auto PC = UCommonGameFlowFunctionLibrary::GetLocalPlayerController(WorldContextObject))
	{
		// Request use card to server
		PC->ServerUseCardByInstanceId(InInstanceId);
	}
}

int32 UCommonGameplayFunctionLibrary::GetPlayerCardNums(const UObject* WorldContextObject, int32 InPlayerId)
{
	if (auto PS = Cast<ATopDownStylePlayerState>(GetPlayerStateById(WorldContextObject, InPlayerId)))
	{
		return PS->GetCardNum();
	}

	return -1;
}

void UCommonGameplayFunctionLibrary::AddWidgetByPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo(WorldContextObject, [WidgetName](ATopDownStylePlayerController* InPC)
	{
		if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
		{
			Manipulator->ClientAddInGameWidget(WidgetName);
		}
	}, InPlayerId);
}

void UCommonGameplayFunctionLibrary::ShowWidgetByPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo(WorldContextObject, [WidgetName](ATopDownStylePlayerController* InPC)
	{
		if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
		{
			Manipulator->ClientShowInGameWidget(WidgetName);
		}
	}, InPlayerId);
}

void UCommonGameplayFunctionLibrary::HideWidgetByPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo(WorldContextObject, [WidgetName](ATopDownStylePlayerController* InPC)
	{
		if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
		{
			Manipulator->ClientShowInGameWidget(WidgetName);
		}
	}, InPlayerId);
}

void UCommonGameplayFunctionLibrary::RemoveWidgetPlayerId(const UObject* WorldContextObject, FString WidgetName, int32 InPlayerId)
{
	ForEachPlayerControllerDo(WorldContextObject, [WidgetName](ATopDownStylePlayerController* InPC)
	{
		if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
		{
			Manipulator->ClientRemoveInGameWidget(WidgetName);
		}
	}, InPlayerId);
}

void UCommonGameplayFunctionLibrary::CreateDamageDisplayByPlayerId(const UObject* WorldContextObject,
	const FDamageDisplayParams& DisplayParams, int32 InPlayerId)
{
	ForEachPlayerControllerDo(WorldContextObject, [&DisplayParams](ATopDownStylePlayerController* InPC)
	{
		if (auto Manipulator = InPC->GetInGameWidgetManipulatorComponent())
		{
			Manipulator->ClientShowDamageWidget(DisplayParams);
		}
	}, InPlayerId);
}

int32 UCommonGameplayFunctionLibrary::GetAlivePlayerNum(const UObject* WorldContextObject)
{
	int32 Result = 0;
	ForEachPlayerStateDo(WorldContextObject, [&Result](ATopDownStylePlayerState* PS)
	{
		if (PS->IsAlive())
		{
			Result++;
		}
	});

	return Result;
}

TArray<int32> UCommonGameplayFunctionLibrary::GetAlivePlayerId(const UObject* WorldContextObject)
{
	TArray<int32> Result;
	ForEachPlayerStateDo(WorldContextObject, [&Result](ATopDownStylePlayerState* PS)
	{
		if (PS->IsAlive())
		{
			Result.Add(PS->GetPlayerId());
		}
	});

	return Result;
}

AGameEffectBase* UCommonGameplayFunctionLibrary::SpawnGameEffectAtPos(const UObject* WorldContextObject, int32 EffectId,
	FVector Pos, FRotator Rot)
{
	if (auto GameInstance = Cast<UDogFightGameInstance>(ULuaIntegrationFunctionLibrary::GetGameInstance(WorldContextObject)))
	{
		if (auto GameEffectService = Cast<UGameEffectService>(GameInstance->GetGameServiceBySuperClass(UGameEffectService::StaticClass())))
		{
			return GameEffectService->SpawnEffectAtPos(EffectId, Pos, Rot);
		}
	}

	return nullptr;
}

void UCommonGameplayFunctionLibrary::DamageActor(const UObject* WorldContextObject, int32 DamageId, AActor* Target, float BaseDamage, AActor* Causer)
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode(WorldContextObject))
	{
		GameMode->DamageActor(DamageId, Target, BaseDamage, Causer);
	}
}

void UCommonGameplayFunctionLibrary::MovePlayerCharacterToPosition(const UObject* WorldContextObject, int32 PlayerId,
	FVector TargetPosition)
{
	ForEachPlayerControllerDo(WorldContextObject, [TargetPosition](ATopDownStylePlayerController* InPC)
	{
		if (auto PlayerPawn = Cast<ATopDownStylePlayerPawn>(InPC->GetPawn()))
		{
			PlayerPawn->ServerMoveCharacter(TargetPosition);
		}
	}, PlayerId);
}

void UCommonGameplayFunctionLibrary::ForEachPlayerStateDo(const UObject* WorldContextObject,
	TFunction<void(ATopDownStylePlayerState*)> ExecuteFunc, int32 PlayerIdMask)
{
	if (auto GameState = GetCurrentTopDownStyleGameState(WorldContextObject))
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

void UCommonGameplayFunctionLibrary::ForEachPlayerControllerDo(const UObject* WorldContextObject,
                                                               TFunction<void(ATopDownStylePlayerController*)> ExecuteFunc, int32 PlayerIdMask)
{
	if (auto GameState = GetCurrentTopDownStyleGameState(WorldContextObject))
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
