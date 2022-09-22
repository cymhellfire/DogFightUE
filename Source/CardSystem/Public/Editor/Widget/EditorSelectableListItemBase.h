#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "EditorSelectableListItemBase.generated.h"

class UBorder;
class UCanvasPanel;
class UContextMenuWidgetBase;

UCLASS(Abstract)
class UEditorSelectableListItemBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	virtual void OpenContextMenu(FVector2D Position);
	virtual void CloseContextMenu();

protected:
	virtual void OnContextMenuOpened() {}

	virtual void OnContextMenuFocusLost();

	UFUNCTION()
	FEventReply OnMouseButtonDownEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

protected:
	UPROPERTY(EditAnywhere, Category="SelectableListItem")
	FSlateBrush DefaultBrush;

	UPROPERTY(EditAnywhere, Category="SelectableListItem")
	FSlateBrush SelectedBrush;

	UPROPERTY(EditAnywhere, Category="SelectableListItem")
	TSubclassOf<UContextMenuWidgetBase> ContextMenuClass;

	UPROPERTY(meta=(BindWidget))
	UBorder* ItemBorder;

	// UPROPERTY(meta=(BindWidgetOptional))
	// UCanvasPanel* ContextMenuContainer;

	UPROPERTY(Transient)
	UContextMenuWidgetBase* ContextMenuWidget;

	bool bContextMenuOpened = false;
	FDelegateHandle ContextMenuFocusLostHandle;
};

