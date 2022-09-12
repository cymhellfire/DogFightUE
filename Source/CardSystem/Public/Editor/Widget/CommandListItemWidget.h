#pragma once

#include "EditorSelectableListItemBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"

#include "CommandListItemWidget.generated.h"

class UCardCommandVisual;
class UWidgetSwitcher;

UCLASS(BlueprintType)
class CARDSYSTEM_API UCommandListItemWidget : public UEditorSelectableListItemBase
{
	GENERATED_BODY()
public:
	UCommandListItemWidget(const FObjectInitializer& ObjectInitializer);

	void InitializeWithCommand(UCardCommandVisual* InCommand);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* CommandNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* CommandIconSwitcher;

};
