#pragma once

#include "LuaGameService.h"
#include "LuaEventService.generated.h"

/**
 * Service that can send event to lua side.
 */
UCLASS()
class LUAINTEGRATION_API ULuaEventService : public ULuaGameService
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category="LuaEventService")
	void SendEventToLua(int32 EventIndex);

	UFUNCTION(BlueprintImplementableEvent, Category="LuaEventService")
	void SendEventToLua_OneParam(int32 EventIndex, const FString& Param);

	UFUNCTION(BlueprintImplementableEvent, Category="LuaEventService")
	void SendEventToLua_OneParam_Int(int32 EventIndex, int32 Param);

	UFUNCTION(BlueprintImplementableEvent, Category="LuaEventService")
	void SendEventToLua_TwoParam_Int(int32 EventIndex, int32 ParamOne, int32 ParamTwo);

	// IUnLuaInterface
	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("LuaIntegration.Services.LuaEventService");
	}
};
