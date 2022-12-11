#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FLuaScriptCreateArgument;

class SCreateLuaScriptWindow : public SCompoundWidget
{
public:
	SCreateLuaScriptWindow();

	SLATE_BEGIN_ARGS(SCreateLuaScriptWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> OnGeneratedModuleOptionWidget(TSharedPtr<FString> InOption);
	TSharedRef<SWidget> OnGeneratedTemplateOptionWidget(TSharedPtr<FString> InOption);
	void OnModuleSelectionChanged(TSharedPtr<FString> InSelectedItem, ESelectInfo::Type SelectInfo);
	void OnTemplateSelectionChanged(TSharedPtr<FString> InSelectedItem, ESelectInfo::Type SelectInfo);

	FReply OnCloseButtonClicked();
	FReply OnCreateButtonClicked();
	FReply OnOpenFileButtonClicked();

	FLuaScriptCreateArgument GetCurrentArgument() const;

	void OnPathTextCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void OnNameTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

	FText GetPreviewPathText() const;

	bool bUseScriptPrefix;
	TOptional<FString> bLastCreateResult;

	TSharedPtr<FString> SelectedModule;
	TSharedPtr<FString> SelectedTemplate;
	FText ScriptPath;
	FText NewScriptName;
	TArray<TSharedPtr<FString>> ModuleNameList;
	TArray<TSharedPtr<FString>> TemplateNameList;
};
