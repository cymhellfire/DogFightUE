#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SCreateLuaScriptWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCreateLuaScriptWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> OnGeneratedTemplateOptionWidget(TSharedPtr<FString> InOption);
	void OnTemplateSelectionChanged(TSharedPtr<FString> InSelectedItem, ESelectInfo::Type SelectInfo);

	FReply OnCloseButtonClicked();
	FReply OnCreateButtonClicked();
	FReply OnOpenFileButtonClicked();

	void OnPathTextCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void OnNameTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

	FText GetPreviewPathText() const;

	TOptional<FString> bLastCreateResult;

	TSharedPtr<FString> SelectedTemplate;
	FText ScriptPath;
	FText NewScriptName;
	TArray<TSharedPtr<FString>> TemplateNameList;
};
