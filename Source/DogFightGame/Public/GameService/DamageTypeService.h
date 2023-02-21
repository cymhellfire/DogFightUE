#pragma once

#include "GameService/LuaGameService.h"
#include "DamageTypeService.generated.h"

UCLASS()
class DOGFIGHTGAME_API UDamageTypeService : public ULuaGameService
{
	GENERATED_BODY()

public:

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.DamageTypeService.DamageTypeService";
	}
};
