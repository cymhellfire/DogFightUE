// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "AbilityCommon.h"
#include "UObject/Object.h"
#include "AbilityDisplayInfo.h"
#include "AbilityBase.generated.h"

class AStandardPlayerState;

/**
 * Base class of all player abilities.
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UAbilityBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void RegisterAbility(AStandardPlayerState* OwnerPlayerState);

	/** Active ability. */
	virtual void Active();

	virtual void BeginDestroy() override;

	bool IsAbilityReady() const { return bReady; }

	FName GetAbilityName() const { return AbilityName; }

	FAbilityDisplayInfo GetAbilityDisplayInfo() const;

	friend bool operator==(const UAbilityBase& Lhs, const UAbilityBase& RHS)
	{
		return Lhs.AbilityName == RHS.AbilityName;
	}

	friend bool operator!=(const UAbilityBase& Lhs, const UAbilityBase& RHS)
	{
		return !(Lhs == RHS);
	}

	void SetAbilitySlot(int32 NewSlot);

	int32 GetAbilitySlot() const { return AbilitySlotIndex; }

	void SetCooldown(int32 NewCooldown);

	void SetAvailability(bool InAvailability);

	virtual bool IsAbilityUsable() const;

protected:

	bool IsAbilityAvailable(EAbilityAvailablePhase CurrentPhase, bool bOwnerRound) const;

	UFUNCTION()
	void OnPrePlayerRoundBegin(int32 PlayerId);

	UFUNCTION()
	void OnPlayerRoundBegin(int32 PlayerId);

	UFUNCTION()
	void OnPlayerDiscardCards(int32 PlayerId);

	UFUNCTION()
	void OnPlayerRoundEnd(int32 PlayerId);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Ability", meta=(DisplayName="Active"))
	void K2_Active();

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilityCooldownChangedSignature, int32, AbilitySlot, int32, Cooldown);
	FAbilityCooldownChangedSignature OnAbilityCooldownChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilityAvailablityChanged, int32, AbilitySlot, bool, NewAvailability);
	FAbilityAvailablityChanged OnAbilityAvailabilityChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	FName AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	FAbilityDisplayInfo AbilityDisplayInfo;

	/** Cooldown of this ability (in unit game round). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	int32 Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	EAbilityCastType AbilityCastType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability", meta=(Bitmask, BitmaskEnum="EAbilityAvailablePhase"))
	int32 AbilityAvailablePhase;

	uint8 bReady:1;
	uint8 bAvailable:1;

	int32 OwnerPlayerId;
	int32 CurrentCooldown;
	int32 AbilitySlotIndex;
};
