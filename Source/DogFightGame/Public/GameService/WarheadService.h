#pragma once

#include "GameService/LuaGameService.h"
#include "WarheadService.generated.h"

UCLASS()
class DOGFIGHTGAME_API UWarheadService : public ULuaGameService
{
	GENERATED_BODY()

public:

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.WarheadService.WarheadService";
	}
};

