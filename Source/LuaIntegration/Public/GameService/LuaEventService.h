#pragma once

#include "LuaEnv.h"
#include "LuaGameService.h"
#include "UnLuaLegacy.h"
#include "UnLuaModule.h"
#include "LuaEventService.generated.h"

/**
 * @brief Send event to lua side with parameters. 
 * @param EventName Event index to send
 * @param ... Variable arguments to send
 */
#define SEND_LUA_EVENT(EventName, ...)		\
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>(this)) \
	{ \
		LuaEventService->SendEventToLua(EventName, __VA_ARGS__); \
	}

/**
 * Service that can send event to lua side.
 */
UCLASS()
class LUAINTEGRATION_API ULuaEventService : public ULuaGameService
{
	GENERATED_BODY()
public:
	/** Generic notify send interface. */
	template <typename... T>
	void SendEventToLua(int32 EventIndex, T&&... Args)
	{
		lua_State* l = IUnLuaModule::Get().GetEnv(this)->GetMainState();
		UnLua::FLuaRetValues RetValue = UnLua::Call(l, "ReceiveNotifyFromC", this, EventIndex, Forward<T>(Args)...);
	}

	// IUnLuaInterface
	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("LuaIntegration.Services.LuaEventService");
	}
};
