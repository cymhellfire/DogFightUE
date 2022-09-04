#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "EditorSelectableListItemBase.generated.h"

class UBorder;

UCLASS(Abstract)
class UEditorSelectableListItemBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

protected:
	UPROPERTY(EditAnywhere, Category="SelectableListItem")
	FSlateBrush DefaultBrush;

	UPROPERTY(EditAnywhere, Category="SelectableListItem")
	FSlateBrush SelectedBrush;

	UPROPERTY(meta=(BindWidget))
	UBorder* ItemBorder;
};

