#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"

#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "PlayerController/TopDownStylePlayerController.h"

TArray<ATopDownStylePlayerController*> UCommonGameFlowFunctionLibrary::GetAllPlayerControllers()
{
	TArray<ATopDownStylePlayerController*> Result;
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		auto PCList = GameMode->GetAllPlayerControllers();
		for (auto PC : PCList)
		{
			if (PC.IsValid())
			{
				Result.Add(PC.Get());
			}
		}
	}

	return Result;
}

void UCommonGameFlowFunctionLibrary::SpawnPlayerCharacterPawn(ATopDownStylePlayerController* Controller)
{
	if (!IsValid(Controller))
	{
		return;
	}

	Controller->SpawnCharacterPawn();
}

void UCommonGameFlowFunctionLibrary::SetCharacterMoveEnableForAllPlayers(bool bEnable)
{
	if (auto GameMode = GetCurrentTopDownStyleGameMode())
	{
		GameMode->SetEnableCharacterMoveForAllPlayers(bEnable);
	}
}

ATopDownStyleGameMode* UCommonGameFlowFunctionLibrary::GetCurrentTopDownStyleGameMode()
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
