#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorMultiTabWidgetBase.generated.h"

class UListView;
class UWidgetSwitcher;

UCLASS(Abstract)
class CARDSYSTEM_API UEditorMultiTabWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

	void AddNewTab(UObject* Item);
	void RemoveTab(UObject* Item);

	void SelectItem(UObject* Item);

	void SelectFirstTab();

protected:
	virtual void InitializeTabItem(UUserWidget* InItem, UObject* InParam) {}
	virtual void InitializeTabWidget(UUserWidget* InWidget, UObject* InParam) {}

	virtual void OnSelectedItemChanged(UObject* SelectedItem) {}

	void OnTabItemSelectionChanged(UObject* InObject);

protected:
	UPROPERTY(meta=(BindWidget))
	UListView* TabItemListView;

	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* TabWidgetSwitcher;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MultiTabWidget", meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> TabWidgetClass;

	UPROPERTY(Transient)
	TArray<UUserWidget*> TabWidgetList;

	int32 SelectedIndex;
	FDelegateHandle TabItemSelectionChangedHandle;
};
