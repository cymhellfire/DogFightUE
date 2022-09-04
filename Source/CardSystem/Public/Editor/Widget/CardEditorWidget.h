#pragma once

#include "CoreMinimal.h"
#include "EditorMultiTabWidgetBase.h"
#include "CardEditorWidget.generated.h"

class UButton;
class FCardEditor;
class FEditingCard;
class UCardVisual;

UCLASS(Abstract)
class CARDSYSTEM_API UCardEditorWidget : public UEditorMultiTabWidgetBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	virtual void BeginDestroy() override;

	void BindCardEditor(TWeakPtr<FCardEditor> InEditor);

protected:
	virtual void InitializeTabWidget(UUserWidget* InWidget, UObject* InParam) override;

	UFUNCTION()
	void OnOpenCommandListPopupButtonClicked();

	UFUNCTION()
	void OnCreateNewCardButtonClicked();

	void OnNewCardCreated(TWeakPtr<FEditingCard> NewCard, int32 FocusId);
	void OnCardClosed(int32 FocusId);

private:
	UPROPERTY(meta=(BindWidget))
	UButton* CreateNewCardButton;

	UPROPERTY(meta=(BindWidget))
	UButton* OpenCommandListPopupButton;

	UPROPERTY(Transient)
	TArray<UCardVisual*> CardVisualList;

	TWeakPtr<FCardEditor> CardEditor;

	FDelegateHandle NewCardCreatedHandle;
	FDelegateHandle CardClosedHandle;
};
