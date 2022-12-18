#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"

UWorld* ULuaIntegrationFunctionLibrary::GetCurrentWorld()
{
	if (auto WorldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport))
	{
		return WorldContext->World();
	}

	return nullptr;
}

UGameInstance* ULuaIntegrationFunctionLibrary::GetGameInstance()
{
	if (auto CurrentWorld = GetCurrentWorld())
	{
		return CurrentWorld->GetGameInstance();
	}

	return nullptr;
}
