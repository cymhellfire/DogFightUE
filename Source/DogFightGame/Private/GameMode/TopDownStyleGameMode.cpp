#include "GameMode/TopDownStyleGameMode.h"
#include "Common/LuaEventDef.h"
#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Pawn/PlayerPawn/TopDownStylePlayerPawn.h"
#include "PlayerController/TopDownStylePlayerController.h"

ATopDownStyleGameMode::ATopDownStyleGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create in-game message sender
	InGameMessageSenderComponent = CreateDefaultSubobject<UInGameMessageSenderComponent>("InGameMessageSender");
}

void ATopDownStyleGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATopDownStyleGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownStyleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Record new player controller
	if (auto PC = Cast<ATopDownStylePlayerController>(NewPlayer))
	{
		AllPlayerControllers.Add(PC);
	}
}

void ATopDownStyleGameMode::PlayerReadyForGame(ATopDownStylePlayerController* InPC)
{
	ReadyPlayerCount++;

	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua_OneParam(ELuaEvent::Type::LuaEvent_ReadyPlayerCount, FString::Printf(TEXT("%d"), ReadyPlayerCount));
	}
}

void ATopDownStyleGameMode::SetEnableCharacterMoveForAllPlayers(bool bEnable)
{
	for (auto PC : AllPlayerControllers)
	{
		if (PC.IsValid())
		{
			if (auto PlayerPawn = Cast<ATopDownStylePlayerPawn>(PC->GetPawn()))
			{
				PlayerPawn->ServerSetCharacterMovable(bEnable);
			}
		}
	}
}
