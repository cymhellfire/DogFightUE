#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"

#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameMode/FreeForAllGameMode.h"
#include "PlayerController/FreeForAllPlayerController.h"

TArray<AFreeForAllPlayerController*> UCommonGameFlowFunctionLibrary::GetAllPlayerControllers()
{
	TArray<AFreeForAllPlayerController*> Result;
	if (auto CurWorld = ULuaIntegrationFunctionLibrary::GetCurrentWorld())
	{
		if (auto GameMode = Cast<AFreeForAllGameMode>(CurWorld->GetAuthGameMode()))
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
	}

	return Result;
}

void UCommonGameFlowFunctionLibrary::SpawnPlayerCharacterPawn(AFreeForAllPlayerController* Controller)
{
	if (!IsValid(Controller))
	{
		return;
	}

	Controller->SpawnCharacterPawn();
}
