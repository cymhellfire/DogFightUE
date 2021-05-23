// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/SlotWidget.h"
#include "Card/GameCardTypes.h"
#include "UI/UIType.h"
#include "CardDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UCardDisplayWidget : public USlotWidget
{
	GENERATED_BODY()

public:

	UCardDisplayWidget(const FObjectInitializer& ObjectInitializer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardSelectionConfirmSignature, TArray<int32>&, SelectionIndexList);
	FCardSelectionConfirmSignature OnCardSelectionConfirmed;

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnCardInfoListChanged();

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnCardUsingAbilityInfoChanged();

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnCardUsableIndexChanged();

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

	/** Set the desired select count for MultiSelection mode. */
	void SetDesireSelectCount(int32 DesireCount);

protected:

	void HandleSelectionChanged();

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FText GetCardNameTextByInfo(const FCardInstanceDisplayInfo& CardInfo) const
	{
		return CardInfo.GetCardNameText();
	}

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	FText GetCardDescriptionTextByInfo(const FCardInstanceDisplayInfo& CardInfo) const
	{
		return CardInfo.GetCardDescriptionText();
	}

	/** Triggered when selected count changed. */
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnSelectedCountChanged(int32 CurrentSelected);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DogFight|CardDisplayWidget")
	ECardSelectionMode CardSelectionMode;

	TArray<int32> SelectedCardIndexList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DogFight|CardDisplayWidget")
	TArray<FCardInstanceDisplayInfo> CardDisplayInfoList;

	/** Are all cards selectable? */
	uint8 bCardItemSelectable : 1;

	/** The desired selection count for MultiSelection mode. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DogFight|CardDisplayWidget")
	int32 DesiredSelectCount;
};
