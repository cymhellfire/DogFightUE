#pragma once

#include "CoreMinimal.h"
#include "EditorTabItemBase.h"
#include "CardEditorTabItemWidget.generated.h"

class URichTextBlock;
class UButton;
class FEditingCard;

UCLASS(Abstract)
class CARDSYSTEM_API UCardEditorTabItemWidget : public UEditorTabItemBase
{
	GENERATED_BODY()
public:
	virtual void BeginDestroy() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void InitializeWithCard(TWeakPtr<FEditingCard> InCard, int32 FocusId);

private:

	void BindCardListener(FEditingCard* InCard);
	void ClearCardListener(FEditingCard* InCard);

	void OnCardNameChanged(FString InName);

	virtual void OnCloseTabButtonClicked() override;

private:
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* CardNameTextBlock;

	int32 BoundId;
	TWeakPtr<FEditingCard> BoundCard;

	FDelegateHandle CardNameChangedHandle;
};
