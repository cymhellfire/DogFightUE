#pragma once

#include "LuaGameService.h"
#include "TimerService.generated.h"

UCLASS()
class LUAINTEGRATION_API UTimerService : public ULuaGameService
{
	GENERATED_BODY()

public:

	virtual FString GetModuleName_Implementation() const override
	{
		return "LuaIntegration.Services.TimerService";
	}
};
