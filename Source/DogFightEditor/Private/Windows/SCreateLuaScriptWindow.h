#pragma once

#include "CoreMinimal.h"
#include "Settings/CreateLuaScriptSettings.h"
#include "Widgets/SCompoundWidget.h"

struct FLuaScriptCreateArgument;
enum class ELuaScriptNameTemplateMode : uint8;

enum class ELuaScriptWindowTab
{
	Default,
	MVVM,
};

class SCreateLuaScriptWindow : public SCompoundWidget
{
public:
	SCreateLuaScriptWindow();

	SLATE_BEGIN_ARGS(SCreateLuaScriptWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> ConstructTabHeaderList();
	TSharedRef<SWidget> ConstructDefaultTab();
	TSharedRef<SWidget> ConstructGroupTab();

	TSharedRef<SWidget> CreateModuleChooseSection();
	TSharedRef<SWidget> CreateSubFolderSection();
	TSharedRef<SWidget> CreateScriptNameSection();
	TSharedRef<SWidget> CreatePathPreviewSection();
	TSharedRef<SWidget> CreateCreateFileSection();

	TSharedRef<SWidget> OnGeneratedModuleOptionWidget(TSharedPtr<FCreateLuaScriptModuleSettings> InOption);
	TSharedRef<SWidget> OnGeneratedTemplateOptionWidget(TSharedPtr<FCreateLuaScriptTemplateSettings> InOption);
	TSharedRef<SWidget> OnGeneratedScriptNameTemplateModeWidget(TSharedPtr<int32> InOption);
	void OnModuleSelectionChanged(TSharedPtr<FCreateLuaScriptModuleSettings> InSelectedItem, ESelectInfo::Type SelectInfo);
	void OnTemplateSelectionChanged(TSharedPtr<FCreateLuaScriptTemplateSettings> InSelectedItem, ESelectInfo::Type SelectInfo);
	void OnScriptNameTemplateModeSelectionChanged(TSharedPtr<int32> InSelectedItem, ESelectInfo::Type SelectInfo);

	FReply OnCloseButtonClicked();
	FReply OnCreateButtonClicked();
	FReply OnOpenFileButtonClicked();

	void SetSelectedTabIndex(int32 InValue);
	void ApplyTabSettings(int32 Index);

	FLuaScriptCreateArgument GetCurrentArgument(int32 Index = 0) const;

	void OnPathTextCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void OnNameTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

	FString GetPreviewPathText(int32 Index) const;

	void SetCreateTemplateList(const FCreateLuaScriptTemplateSettings& InTemplate);
	void AddCreateTemplateList(const FCreateLuaScriptTemplateSettings& InTemplate);
	void ClearCreateTemplateList();

	bool bUseScriptPrefix;
	TOptional<FString> OverrideFolder;
	TOptional<FString> bLastCreateResult;
	TOptional<FString> HardcodedSubfolder;

	TSharedPtr<FCreateLuaScriptModuleSettings> SelectedModule;
	TSharedPtr<FCreateLuaScriptTemplateSettings> SelectedTemplate;
	FText ScriptPath;
	FText NewScriptName;
	TArray<FCreateLuaScriptTabSettings> TabSettingList;
	TArray<TSharedPtr<FCreateLuaScriptModuleSettings>> ModuleSettingList;
	TArray<TSharedPtr<FCreateLuaScriptTemplateSettings>> TemplateSettingList;
	TArray<TSharedPtr<int32>> ScriptNameTemplateModeOptions;
	TArray<FString> ScriptNameTemplateModeDesc;
	ELuaScriptNameTemplateMode SelectedTemplateMode;

	TArray<FCreateLuaScriptTemplateSettings> CreateTemplateList;

	ELuaScriptWindowTab SelectedTab;
	int32 SelectTabIndex;
	FMargin DefaultRowPadding;
};
