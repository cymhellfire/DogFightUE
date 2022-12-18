#include "GameMode/FreeForAllGameMode.h"
#include "Common/LuaEventDef.h"
#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "PlayerController/FreeForAllPlayerController.h"

AFreeForAllGameMode::AFreeForAllGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create in-game message sender
	InGameMessageSenderComponent = CreateDefaultSubobject<UInGameMessageSenderComponent>("InGameMessageSender");
}

void AFreeForAllGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFreeForAllGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AFreeForAllGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Record new player controller
	if (auto PC = Cast<AFreeForAllPlayerController>(NewPlayer))
	{
		AllPlayerControllers.Add(PC);
	}
}

void AFreeForAllGameMode::PlayerReadyForGame(AFreeForAllPlayerController* InPC)
{
	ReadyPlayerCount++;

	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua_OneParam(ELuaEvent::Type::LuaEvent_ReadyPlayerCount, FString::Printf(TEXT("%d"), ReadyPlayerCount));
	}
}
