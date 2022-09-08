#pragma once

#include "CoreMinimal.h"
#include "EditorTabItemBase.h"
#include "CardFunctionTabItemWidget.generated.h"

class URichTextBlock;
class FEditingCardFunction;

UCLASS(Abstract)
class CARDSYSTEM_API UCardFunctionTabItemWidget : public UEditorTabItemBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void InitializeWithFunction(TWeakPtr<FEditingCardFunction> InFunction);

protected:
	void BindFunctionListener(FEditingCardFunction* InFunction);
	void ClearFunctionListener(FEditingCardFunction* InFunction);

	void OnFunctionNameChanged(FString InName);

	virtual void OnCloseTabButtonClicked() override;

private:
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* FunctionNameTextBlock;

	TWeakPtr<FEditingCardFunction> BoundFunction;

	FDelegateHandle FunctionNameChangedHandle;
};
