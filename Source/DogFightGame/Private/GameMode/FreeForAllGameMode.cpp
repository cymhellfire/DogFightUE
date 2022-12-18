#include "GameMode/FreeForAllGameMode.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"

void AFreeForAllGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFreeForAllGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AFreeForAllGameMode::PlayerReadyForGame(AFreeForAllPlayerController* InPC)
{
	ReadyPlayerCount++;

	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua_OneParam(TEXT("LuaEvent_ReadyPlayerCount"), FString::Printf(TEXT("%d"), ReadyPlayerCount));
	}
}
