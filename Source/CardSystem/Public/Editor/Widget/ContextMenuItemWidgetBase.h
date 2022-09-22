#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "ContextMenuItemWidgetBase.generated.h"

class UContextMenuItemData;
class URichTextBlock;

UCLASS(Abstract)
class CARDSYSTEM_API UContextMenuItemWidgetBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	virtual void InitializeWithItemData(UContextMenuItemData* InItemData);

protected:

	TWeakObjectPtr<UContextMenuItemData> ItemData;

private:
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* ItemNameTextBlock;
};
