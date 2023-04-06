#include "FunctionLibrary/CommonFunctionLibraryBase.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameMode/TopDownStyleGameState.h"
#include "GameMode/TopDownStyleGameMode.h"

ATopDownStyleGameMode* UCommonFunctionLibraryBase::GetCurrentTopDownStyleGameMode(const UObject* WorldContextObject)
{
	if (auto CurWorld = ULuaIntegrationFunctionLibrary::GetCurrentWorld(WorldContextObject))
	{
		if (auto GameMode = Cast<ATopDownStyleGameMode>(CurWorld->GetAuthGameMode()))
		{
			return GameMode;
		}
	}

	return nullptr;
}

ATopDownStyleGameState* UCommonFunctionLibraryBase::GetCurrentTopDownStyleGameState(const UObject* WorldContextObject)
{
	if (auto CurWorld = ULuaIntegrationFunctionLibrary::GetCurrentWorld(WorldContextObject))
	{
		return Cast<ATopDownStyleGameState>(CurWorld->GetGameState());
	}

	return nullptr;
}
