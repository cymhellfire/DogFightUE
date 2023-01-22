#pragma once

#include "GameService/LuaGameService.h"
#include "GameWidgetService.generated.h"

UCLASS()
class DOGFIGHTGAME_API UGameWidgetService : public ULuaGameService
{
	GENERATED_BODY()

public:

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.GameWidgetService";
	}
};
