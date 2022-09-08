#pragma once

#include "CoreMinimal.h"
#include "EditorMultiTabWidgetBase.h"
#include "CardEditorTabWidget.generated.h"

class FEditingCard;
class FEditingCardFunction;
class UListView;
class UButton;
class URichTextBlock;
class UCardFunctionVisual;

UCLASS(Abstract)
class CARDSYSTEM_API UCardEditorTabWidget : public UEditorMultiTabWidgetBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

	void BindEditingCard(TWeakPtr<FEditingCard> InCard);
protected:

	void BindCardListener(TWeakPtr<FEditingCard> InCard);
	void ClearCardListener(TWeakPtr<FEditingCard> InCard);

	void ClearCardFunctions();

	void OnCardNameChanged(FString InName);
	void OnCardFunctionAdded(TWeakPtr<FEditingCardFunction> InFunction);

	UFUNCTION()
	void OnCardRenameButtonClicked();
	void OnCardRenamePopupConfirmed(FText InText);

	UFUNCTION()
	void OnCreateNewCardFunctionButtonClicked();
	void OnCreateNewCardFunctionPopupConfirmed(FText InText);

private:
	UPROPERTY(meta=(BindWidget))
	UButton* CreateNewCardFunctionButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CardRenameButton;

	UPROPERTY(meta=(BindWidget))
	URichTextBlock* CardNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UListView* CardFunctionsListView;

	TWeakPtr<FEditingCard> EditingCard;

	UPROPERTY(Transient)
	TArray<UCardFunctionVisual*> CardFunctionVisualList;

	FDelegateHandle CardNameChangedHandle;
	FDelegateHandle CardFunctionAddedHandle;
};
