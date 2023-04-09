#pragma once

#include "GameService/LuaGameService.h"
#include "CardGeneratorService.generated.h"

UCLASS()
class DOGFIGHTGAME_API UCardGeneratorService : public ULuaGameService
{
	GENERATED_BODY()
public:
	

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.CardService.CardGeneratorService";
	}
};
