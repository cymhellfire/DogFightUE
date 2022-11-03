#include "GameService/LuaGameService.h"

void ULuaGameService::Startup()
{
	Super::Startup();

	// Invoke lua side function
	StartupScript(GetClass()->GetName());
}

void ULuaGameService::Shutdown()
{
	Super::Shutdown();

	// Invoke lua side function
	ShutdownScript(GetClass()->GetName());
}
