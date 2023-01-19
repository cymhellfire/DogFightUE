﻿#include "FunctionLibrary/CommonFunctionLibraryBase.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameMode/TopDownStyleGameState.h"
#include "GameMode/TopDownStyleGameMode.h"

ATopDownStyleGameMode* UCommonFunctionLibraryBase::GetCurrentTopDownStyleGameMode()
{
	if (auto CurWorld = ULuaIntegrationFunctionLibrary::GetCurrentWorld())
	{
		if (auto GameMode = Cast<ATopDownStyleGameMode>(CurWorld->GetAuthGameMode()))
		{
			return GameMode;
		}
	}

	return nullptr;
}

ATopDownStyleGameState* UCommonFunctionLibraryBase::GetCurrentTopDownStyleGameState()
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		return GameMode->GetGameState<ATopDownStyleGameState>();
	}

	return nullptr;
}
