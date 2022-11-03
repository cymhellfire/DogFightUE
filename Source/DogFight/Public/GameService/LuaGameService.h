#pragma once

#include "UnLuaInterface.h"
#include "GameService/GameService.h"
#include "LuaGameService.generated.h"

UCLASS(Abstract)
class DOGFIGHT_API ULuaGameService : public UGameService, public IUnLuaInterface
{
	GENERATED_BODY()
public:
	virtual void Startup() override;
	virtual void Shutdown() override;

	/** Startup function for Lua script overriding. */
	UFUNCTION(BlueprintImplementableEvent, Category="LuaGameService")
	void StartupScript(const FString& ServiceName);

	/** Shutdown function for Lua script overriding. */
	UFUNCTION(BlueprintImplementableEvent, Category="LuaGameService")
	void ShutdownScript(const FString& ServiceName);
};
