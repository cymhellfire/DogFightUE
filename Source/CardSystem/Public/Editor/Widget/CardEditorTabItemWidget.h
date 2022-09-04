#pragma once

#include "CoreMinimal.h"
#include "EditorSelectableListItemBase.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "CardEditorTabItemWidget.generated.h"

class URichTextBlock;
class UButton;
class FEditingCard;

UCLASS(Abstract)
class CARDSYSTEM_API UCardEditorTabItemWidget : public UEditorSelectableListItemBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void InitializeWithCard(TWeakPtr<FEditingCard> InCard, int32 FocusId);

private:

	void BindCardListener(FEditingCard* InCard);
	void ClearCardListener(FEditingCard* InCard);

	void OnCardNameChanged(FString InName);

	UFUNCTION()
	void OnCloseTabButtonClicked();

private:
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* CardNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UButton* CloseTabButton;

	int32 BoundId;
	TWeakPtr<FEditingCard> BoundCard;

	FDelegateHandle CardNameChangedHandle;
};
