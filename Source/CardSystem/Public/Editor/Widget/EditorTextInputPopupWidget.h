#pragma once

#include "CoreMinimal.h"
#include "EditorPopupWidgetBase.h"
#include "Common/CardEditorType.h"
#include "EditorTextInputPopupWidget.generated.h"

class UEditableTextBox;
class URichTextBlock;
class USizeBox;

UCLASS(Abstract)
class CARDSYSTEM_API UEditorTextInputPopupWidget : public UEditorPopupWidgetBase
{
	GENERATED_BODY()
public:

	virtual void OnConfirmButtonClicked() override;

	virtual void Close() override;

	void ApplyParam(const FTextInputPopupParameter& InParam);

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FInputTextConfirmSignature, FText);
	FInputTextConfirmSignature OnInputTextConfirmed;

private:
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* InputTextBlock;

	UPROPERTY(meta=(BindWidget))
	URichTextBlock* TitleTextBlock;

	UPROPERTY(meta=(BindWidget))
	USizeBox* PopupSizeBox;
};
