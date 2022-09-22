#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContextMenuWidgetBase.generated.h"

class UListView;
class UContextMenuItemData;

UCLASS(Abstract)
class CARDSYSTEM_API UContextMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	void InitializeItemList(const TArray<UContextMenuItemData*>& InItemList);

protected:
	virtual void OnMenuItemClicked(UObject* Item);

	virtual void ClearAllItems();

public:

	DECLARE_MULTICAST_DELEGATE(FContextMenuSimpleSignature);
	FContextMenuSimpleSignature OnContextMenuItemClicked;
	FContextMenuSimpleSignature OnContextMenuFocusLost;

protected:
	UPROPERTY(meta=(BindWidget))
	UListView* ContextMenuItemListView;

	UPROPERTY(Transient)
	TArray<UContextMenuItemData*> MenuItemDataList;

private:
	FDelegateHandle MenuItemClickedHandle;
};
