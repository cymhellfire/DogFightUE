// Dog Fight Game Code By CYM.


#include "Ability/AbilityBase.h"
#include "Game/StandardPlayerState.h"
#include "Game/StandardGameMode.h"

void UAbilityBase::RegisterAbility(AStandardPlayerState* OwnerPlayerState)
{
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		StandardGameMode->OnPlayerRoundEnd.AddDynamic(this, &UAbilityBase::OnPlayerRoundEnd);
	}

	OwnerPlayerId = OwnerPlayerState->GetPlayerId();
}

void UAbilityBase::Active()
{
	// Set cooldown if larger than zero
	if (Cooldown > 0)
	{
		SetCooldown(Cooldown);
	}

	K2_Active();
}

void UAbilityBase::BeginDestroy()
{
	UWorld* CurrentWorld = GetWorld();
	if (IsValid(CurrentWorld))
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(CurrentWorld->GetAuthGameMode()))
		{
			StandardGameMode->OnPlayerRoundEnd.RemoveDynamic(this, &UAbilityBase::OnPlayerRoundEnd);
		}
	}

	Super::BeginDestroy();
}

FAbilityDisplayInfo UAbilityBase::GetAbilityDisplayInfo() const
{
	FAbilityDisplayInfo NewInfo(AbilityDisplayInfo);
	// Fill parameters which are not given
	NewInfo.CastType = AbilityCastType;
	NewInfo.AbilityCooldown = Cooldown;

	return NewInfo;
}

void UAbilityBase::SetAbilitySlot(int32 NewSlot)
{
	AbilitySlotIndex = NewSlot;
}

void UAbilityBase::SetCooldown(int32 NewCooldown)
{
	if (CurrentCooldown == NewCooldown)
		return;

	CurrentCooldown = NewCooldown;
	bReady = CurrentCooldown == 0;
	OnAbilityCooldownChanged.Broadcast(AbilitySlotIndex, CurrentCooldown);
}

void UAbilityBase::OnPlayerRoundEnd(int32 PlayerId)
{
	if (OwnerPlayerId == PlayerId)
	{
		SetCooldown(CurrentCooldown - 1);
	}
}
