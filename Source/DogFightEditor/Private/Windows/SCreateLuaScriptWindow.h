#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FLuaScriptCreateArgument;

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
	TSharedRef<SWidget> ConstructMVVMTab();

	TSharedRef<SWidget> CreateModuleChooseSection();
	TSharedRef<SWidget> CreateSubFolderSection();
	TSharedRef<SWidget> CreateScriptNameSection();
	TSharedRef<SWidget> CreatePathPreviewSection();
	TSharedRef<SWidget> CreateCreateFileSection();

	TSharedRef<SWidget> OnGeneratedModuleOptionWidget(TSharedPtr<FString> InOption);
	TSharedRef<SWidget> OnGeneratedTemplateOptionWidget(TSharedPtr<FString> InOption);
	void OnModuleSelectionChanged(TSharedPtr<FString> InSelectedItem, ESelectInfo::Type SelectInfo);
	void OnTemplateSelectionChanged(TSharedPtr<FString> InSelectedItem, ESelectInfo::Type SelectInfo);

	FReply OnCloseButtonClicked();
	FReply OnCreateButtonClicked();
	FReply OnOpenFileButtonClicked();

	void OnSwitchDefaultTab();
	void OnSwitchMVVMTab();

	FLuaScriptCreateArgument GetCurrentArgument(int32 Index = 0) const;

	void OnPathTextCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void OnNameTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

	FString GetPreviewPathText(int32 Index) const;

	void SetCreateTemplateList(FString InTemplate);
	void AddCreateTemplateList(FString InTemplate);
	void ClearCreateTemplateList();

	bool bUseScriptPrefix;
	TOptional<FString> OverrideFolder;
	TOptional<FString> bLastCreateResult;
	TOptional<FString> HardcodedSubfolder;

	TSharedPtr<FString> SelectedModule;
	TSharedPtr<FString> SelectedTemplate;
	FText ScriptPath;
	FText NewScriptName;
	TArray<TSharedPtr<FString>> ModuleNameList;
	TArray<TSharedPtr<FString>> TemplateNameList;

	TArray<FString> CreateTemplateList;

	ELuaScriptWindowTab SelectedTab;
	FMargin DefaultRowPadding;
};
