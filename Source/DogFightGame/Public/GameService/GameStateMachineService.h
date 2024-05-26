﻿// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "GameStateMachineService.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UGameStateMachineService : public ULuaGameService
{
	GENERATED_BODY()

public:
	virtual void Startup() override;
	virtual void Shutdown() override;

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.GameStateMachineService.GameStateMachineService";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameInstanceStartScript();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPostLoadMapScript(const FString& MapName);

private:
	/**
	 * Convert given map path to short version.
	 * @param InMapPath Origin map path string.
	 * @return Short version map name.
	 */
	UFUNCTION(BlueprintCallable)
	FString GetShortMapName(const FString& InMapPath) const;

private:
	void OnPreLoadMap(const FWorldContext& WorldContext, const FString& MapName);
	void OnPostLoadMap(UWorld* InWorld);
};
