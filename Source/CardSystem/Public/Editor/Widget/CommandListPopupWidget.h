#pragma once

#include "CoreMinimal.h"
#include "EditorPopupWidgetBase.h"
#include "CommandListPopupWidget.generated.h"

class UListView;
class UButton;
class UScrollBox;
class URichTextBlock;
class UContentWidget;
class UCardCommandVisual;
class UCommandListItemWidget;

UCLASS(Abstract)
class CARDSYSTEM_API UCommandListPopupWidget : public UEditorPopupWidgetBase
{
	GENERATED_BODY()
public:

	virtual bool Initialize() override;

	virtual void BeginDestroy() override;

	void SetClassList(TArray<UCardCommandVisual*> InClassList);

private:

	void ShowCommandListView();
	void HideCommandListView();

	void OnCommandListItemSelectedChanged(UObject* Item);

	UFUNCTION()
	void OnCommandListTriggerClicked();

	virtual void OnConfirmButtonClicked() override;

	virtual void OnCancelButtonClicked() override;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandItemConfirm, int32);
	FOnCommandItemConfirm OnCommandItemConfirm;

private:

	UPROPERTY(meta=(BindWidget))
	UCommandListItemWidget* SelectedItemVisual;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* CommandListScrollBox;

	UPROPERTY(meta=(BindWidget))
	UListView* CommandListView;

	UPROPERTY(meta=(BindWidget))
	URichTextBlock* CommandDescTextBlock;

	UPROPERTY(meta=(BindWidget))
	UContentWidget* CommandListViewContainer;

	UPROPERTY(meta=(BindWidget))
	UButton* CommandListViewTrigger;

	FDelegateHandle CommandListItemSelectedHandle;

	uint8 bCommandListOpened : 1;

	int32 SelectedCommandId;
};
