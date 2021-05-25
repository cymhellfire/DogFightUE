// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Ability/AbilityDisplayInfo.h"
#include "UI/Widget/SlotWidget.h"
#include "AbilityPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityPanelWidget : public USlotWidget
{
	GENERATED_BODY()

public:

	void AddAbility(FAbilityDisplayInfo NewAbilityInfo, int32 AbilitySlot);

	void RemoveAbility(int32 AbilityToRemove);

	void UpdateAbilityCooldownAtSlot(int32 AbilitySlot, int32 CurrentCooldown);

	void UpdateAbilityAvailability(int32 AbilitySlot, bool NewAvailability);

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void UseAbilityAtSlot(int32 AbilitySlot);

protected:

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta=(DisplayName="Add Ability"))
	void K2_AddAbility(const FAbilityDisplayInfo& NewAbilityInfo, int32 AbilitySlot);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta=(DisplayName="Remove Ability"))
	void K2_RemoveAbility(int32 AbilityToRemove);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta=(DisplayName="Update Ability Cooldown"))
	void K2_UpdateAbilityCooldown(int32 AbilitySlot, int32 CurrentCooldown);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI", meta=(DisplayName="Update Ability Availability"))
	void K2_UpdateAbilityAvailability(int32 AbilitySlot, bool NewAvailability);

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityPanelSelected, int32, SelectedSlot);
	FAbilityPanelSelected OnAbilitySelected;
};
