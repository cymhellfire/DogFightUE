#pragma once

#include "CoreMinimal.h"
#include "EditorSelectableListItemBase.h"
#include "FunctionCommandListItemWidget.generated.h"

class URichTextBlock;
class UCardCommandEditingData;
class FEditingCardCommand;

UCLASS(Abstract)
class CARDSYSTEM_API UFunctionCommandListItemWidget : public UEditorSelectableListItemBase
{
	GENERATED_BODY()
public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	void InitializeWithEditingCommand(UCardCommandEditingData* InData);

	void BindCommandListener(FEditingCardCommand* InCommand);
	void ClearCommandListener(FEditingCardCommand* InCommand);

	void OnCommandIdChanged(int32 InId);

	void OnContextMenuItemClicked();

	virtual void OnContextMenuOpened() override;

private:
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* CommandContextTextBlock;

	TWeakPtr<FEditingCardCommand> EditingCommand;

	FDelegateHandle CommandIdChangedHandle;
	FDelegateHandle ContextMenuItemClickedHandle;
};
