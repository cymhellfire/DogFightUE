// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "AvatarManagerService.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UAvatarManagerService : public ULuaGameService
{
	GENERATED_BODY()

public:

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.AvatarManagerService.AvatarManagerService";
	}
};
