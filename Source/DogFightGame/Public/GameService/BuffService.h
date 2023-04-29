// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "BuffService.generated.h"

class UNewBuffBase;

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UBuffService : public ULuaGameService
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="BuffGameService")
	UNewBuffBase* CreateBuff(int32 BuffId);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.BuffService.BuffService";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="BuffGameService")
	UNewBuffBase* CreateBuffById(int32 BuffId);
};
