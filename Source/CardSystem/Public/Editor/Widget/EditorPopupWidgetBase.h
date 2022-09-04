#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorPopupWidgetBase.generated.h"

class UButton;

UCLASS(Abstract)
class CARDSYSTEM_API UEditorPopupWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual bool Initialize() override;

	virtual void BeginDestroy() override;

	virtual void Close();

protected:
	UFUNCTION()
	virtual void OnConfirmButtonClicked();

	UFUNCTION()
	virtual void OnCancelButtonClicked();

protected:
	UPROPERTY(meta=(BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;
};
