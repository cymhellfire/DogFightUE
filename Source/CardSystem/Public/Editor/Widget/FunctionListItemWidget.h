#pragma once

#include "CoreMinimal.h"
#include "EditorSelectableListItemBase.h"
#include "Blueprint/UserWidget.h"
#include "FunctionListItemWidget.generated.h"

class URichTextBlock;
class UCardFunctionVisual;

UCLASS(Blueprintable)
class CARDSYSTEM_API UFunctionListItemWidget : public UEditorSelectableListItemBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void InitializeWithVisual(UCardFunctionVisual* FunctionVisual);

private:
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* FunctionNameTextBlock;
};
