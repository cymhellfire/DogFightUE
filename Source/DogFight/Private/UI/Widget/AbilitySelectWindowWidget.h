// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/SlotWidget.h"
#include "Ability/AbilityDisplayInfo.h"
#include "AbilitySelectWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class UAbilitySelectWindowWidget : public USlotWidget
{
	GENERATED_BODY()

public:
	void SetDisplayAbilities(TArray<FAbilityDisplayInfo> AbilityDisplayInfos);

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void SelectAbilityAtIndex(int32 Index);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta=(DisplayName="Display Abilities Changed"))
	void K2_DisplayAbilitiesChanged(const TArray<FAbilityDisplayInfo>& AbilityDisplayInfos);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelectedSignature, int32, SelectedIndex);
	FAbilitySelectedSignature OnAbilitySelected;
};
