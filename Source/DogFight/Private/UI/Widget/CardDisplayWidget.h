// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizableCardTypes.h"
#include "UIType.h"
#include "CardDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UCardDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UCardDisplayWidget(const FObjectInitializer& ObjectInitializer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardSelectionConfirmSignature, const TArray<int32>&, SelectionIndexList);
	FCardSelectionConfirmSignature OnCardSelectionConfirmed;

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnCardInfoListChanged();

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnCardSelectionModeChanged(ECardSelectionMode NewMode);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnCardItemsActiveChanged(bool Active);

	void SetCardDisplayInfos(const TArray<FCardInstanceDisplayInfo>& CardDisplayInfos);

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void SelectCardByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void DeselectCardByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void ConfirmSelection();

	void SetSelectMode(ECardSelectionMode NewMode);

	void SetCardItemsSelectable(bool bSelectable);

protected:

	void HandleSelectionChanged();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DogFight|CardDisplayWidget")
	ECardSelectionMode CardSelectionMode;

	TArray<int32> SelectedCardIndexList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DogFight|CardDisplayWidget")
	TArray<FCardInstanceDisplayInfo> CardDisplayInfoList;

	bool bCardItemSelectable;
};
