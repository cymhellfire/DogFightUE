#pragma once

#include "ImGuiDelegates.h"
#include "GameService/LuaGameService.h"
#include "DebugPanelService.generated.h"

UCLASS()
class DOGFIGHTGAME_API UDebugPanelService : public ULuaGameService
{
	GENERATED_BODY()
public:
	UDebugPanelService();

	virtual void Startup() override;
	virtual void Shutdown() override;

	virtual FString GetModuleName_Implementation() const override
	{
		return "";
	}

	void ToggleDebugPanel();

private:
	void ImGuiTick();

	void OnDebugPanelEnableChanged();

	UFUNCTION()
	void OnDelayRegisterTimerExpired();

private:
	uint8 bDebugPanelEnable:1;

	FTimerHandle DelayRegisterTimerHandle;
	FImGuiDelegateHandle ImGuiTickHandle;
};
