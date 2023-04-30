#pragma once

#include "UnLuaInterface.h"
#include "GameService/GameService.h"
#include "LuaGameService.generated.h"

/**
 * Basic class for all game services that need bind lua script.
 */
UCLASS(Abstract)
class LUAINTEGRATION_API ULuaGameService : public UGameService, public IUnLuaInterface
{
	GENERATED_BODY()
public:
	virtual void Startup() override;
	virtual void Shutdown() override;

	virtual void PostStartup() override;

	virtual FName GetRegisterName() const override;

	/** Startup function for Lua script overriding. */
	UFUNCTION(BlueprintImplementableEvent, Category="LuaGameService")
	void StartupScript(const FString& ServiceName);

	/** Shutdown function for Lua script overriding. */
	UFUNCTION(BlueprintImplementableEvent, Category="LuaGameService")
	void ShutdownScript(const FString& ServiceName);

	UFUNCTION(BlueprintImplementableEvent, Category="LuaGameService")
	void PostStartupScript();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuaGameService")
	bool bRegisterNativeClassName;
};
