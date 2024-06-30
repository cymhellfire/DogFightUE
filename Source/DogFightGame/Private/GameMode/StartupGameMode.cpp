// Dog Fight Game Code By CYM.


#include "GameMode/StartupGameMode.h"

#include "Common/LuaEventDef.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"

void AStartupGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Send the startup event
	if (auto EventService = UGameService::GetGameService<ULuaEventService>(this))
	{
		EventService->SendEventToLua(ELuaEvent::LuaEvent_GameStartup);
	}
}
