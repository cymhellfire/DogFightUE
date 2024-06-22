#pragma once

#include "ImGuiDelegates.h"
#include "GameService/LuaGameService.h"
#include "DebugPanelService.generated.h"

DECLARE_DYNAMIC_DELEGATE(FDebugPanelControlDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDebugPanelExecuteTextDelegate, FString, Params);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDebugPanelExecuteComboBoxDelegate, int32, Index);

struct FDebugPanelControlBase
{
	virtual ~FDebugPanelControlBase() {}

	FName TabKey;

	virtual void DrawControl() = 0;
};

struct FDebugPanelSameLineControl : public FDebugPanelControlBase
{
	float Spacing = 0;

	virtual void DrawControl() override;
};

struct FDebugPanelTextControl : public FDebugPanelControlBase
{
	FString Context;

	virtual void DrawControl() override;
};

struct FDebugPanelButtonControl : public FDebugPanelControlBase
{
	FString Context;
	FDebugPanelControlDelegate ClickCallback;

	virtual void DrawControl() override;
};

struct FDebugPanelExecuteTextControl : public FDebugPanelControlBase
{
	FString Context;
	FDebugPanelExecuteTextDelegate ClickCallback;

	virtual void DrawControl() override;
};

USTRUCT(BlueprintType)
struct FDebugPanelComboBoxEntryInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;
};

struct FDebugPanelExecuteComboBoxControl : public FDebugPanelControlBase
{
	FDebugPanelExecuteComboBoxControl();
	virtual ~FDebugPanelExecuteComboBoxControl() override;

	int32 SelectedIndex;
	FString Context;
	FVector2D ListSize;
	FDebugPanelExecuteComboBoxDelegate ClickCallback;
	TArray<FDebugPanelComboBoxEntryInfo> EntryList;

	virtual void DrawControl() override;
};

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
		return "DogFight.Services.DebugPanelService.DebugPanelService";
	}

	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void ToggleDebugPanel();

	void ToggleImPlotDemo();

	/**
	 * Setup a tab name which will act as the default tab to place following debug controls.
	 * @param InTabName		The name of tab.
	 */
	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void SetDrawingTab(const FName& InTabName);

	/**
	 * Keep next debug control in same line.
	 * @param Spacing		Spacing between next and previous control.
	 * @param TabKey		The key of tab this control is placed.
	 */
	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void KeepSameLine(float Spacing = 0, const FName& TabKey = "Default");

	/**
	 * Add a text control to debug panel.
	 * @param Context		Text context to display.
	 * @param TabKey		The key of tab this control is placed.
	 */
	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void AddText(const FString& Context, const FName& TabKey = "Default");

	/**
	 * Add a button control to debug panel.
	 * @param Context		Text context on button.
	 * @param Callback		Click callback function.
	 * @param TabKey		The key of tab this control is placed.
	 */
	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void AddButton(const FString& Context, FDebugPanelControlDelegate Callback, const FName& TabKey = "Default");

	/**
	 * Add a input box with a confirm button.
	 * Click callback will be triggered with entered text.
	 * @param Context		Text context on button.
	 * @param Callback		Click callback function.
	 * @param TabKey		The key of tab this control is placed.
	 */
	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void AddExecuteText(const FString& Context, FDebugPanelExecuteTextDelegate Callback, const FName& TabKey = "Default");

	/**
	 * Add a selectable list view with given entry information.
	 * Click callback will be triggered with selected entry index.
	 * @param Context		Text context on button.
	 * @param EntryInfos	Info list of all selectable entry.
	 * @param Callback		Click callback function.
	 * @param Size			The list view size.
	 * @param TabKey		The key of tab this control is placed.
	 */
	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void AddExecuteComboBox(const FString& Context, TArray<FDebugPanelComboBoxEntryInfo> EntryInfos,
		FDebugPanelExecuteComboBoxDelegate Callback, FVector2D Size = FVector2D(120, 200), const FName& TabKey = "Default");

protected:
	UFUNCTION(BlueprintCallable, Category="DebugPanelService")
	void RebuildPanel();

	UFUNCTION(BlueprintImplementableEvent, Category="DebugPanelService")
	void GatherAllControlParams();

	void ClearAllControlParams();

private:
	void ImGuiTick();

	void OnDebugPanelEnableChanged();

	UFUNCTION()
	void OnDelayRegisterTimerExpired();

	void RecordNewControlParam(TSharedPtr<FDebugPanelControlBase> InParams);

	void DrawTab(const FName& InTabName);

	/**
	 * Get the actual tab name to place new debug controls.
	 * @param InName		Check name.
	 * @return				Actual name.
	 */
	FName GetActualTabName(const FName& InName) const;

private:
	uint8 bDebugPanelEnable:1;
	uint8 bImPlotDemoEnable:1;

	FName DrawingTabName;

	FTimerHandle DelayRegisterTimerHandle;
	FImGuiDelegateHandle ImGuiTickHandle;

	TMap<FName, TArray<TSharedPtr<FDebugPanelControlBase>>> DebugPanelControlMap;
};
