// Dog Fight Game Code By CYM.

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

	virtual void Tick(float DeltaTime) override;

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.GameStateMachineService.GameStateMachineService";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameInstanceStartScript();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTickScript(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPostLoadMapScript(const FString& MapName);

private:
	void OnPreLoadMap(const FWorldContext& WorldContext, const FString& MapName);
	void OnPostLoadMap(UWorld* InWorld);
};
