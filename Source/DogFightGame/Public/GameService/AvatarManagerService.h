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
	/**
	 * Get AvatarDesc with given config id.
	 * @param ConfigId Id of avatar config to get.
	 * @return Path of AvatarDesc asset.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="AvatarManagerService")
	FString GetAvatarDescAssetPathByConfigId(int32 ConfigId);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.AvatarManagerService.AvatarManagerService";
	}
};
