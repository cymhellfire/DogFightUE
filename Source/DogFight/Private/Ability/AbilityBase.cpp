// Dog Fight Game Code By CYM.


#include "Ability/AbilityBase.h"
#include "Game/StandardPlayerState.h"
#include "Game/StandardGameMode.h"
#include "Common/BitmaskOperation.h"

void UAbilityBase::RegisterAbility(AStandardPlayerState* OwnerPlayerState)
{
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (AbilityCastType == EAbilityCastType::ACT_Aggressive)
		{
			StandardGameMode->OnPlayerRoundEnd.AddDynamic(this, &UAbilityBase::OnPlayerRoundEnd);

			StandardGameMode->OnPrePlayerRoundBegin.AddDynamic(this, &UAbilityBase::UAbilityBase::OnPrePlayerRoundBegin);
			StandardGameMode->OnPlayerRoundBegin.AddDynamic(this, &UAbilityBase::OnPlayerRoundBegin);
			StandardGameMode->OnPlayerDiscardCard.AddDynamic(this, &UAbilityBase::OnPlayerDiscardCards);
		}
	}

	OwnerPlayerId = OwnerPlayerState->GetPlayerId();

	// Set cooldown to zero at start
	SetCooldown(0);
	if (AbilityCastType == EAbilityCastType::ACT_Passive)
	{
		// Set availability to true if this is a passive ability
		SetAvailability(true);
	}
}

void UAbilityBase::Active()
{
	if (!IsAbilityUsable())
		return;

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
			if (AbilityCastType == EAbilityCastType::ACT_Aggressive)
			{
				StandardGameMode->OnPlayerRoundEnd.RemoveDynamic(this, &UAbilityBase::OnPlayerRoundEnd);

				StandardGameMode->OnPrePlayerRoundBegin.RemoveDynamic(this, &UAbilityBase::UAbilityBase::OnPrePlayerRoundBegin);
				StandardGameMode->OnPlayerRoundBegin.RemoveDynamic(this, &UAbilityBase::OnPlayerRoundBegin);
				StandardGameMode->OnPlayerDiscardCard.RemoveDynamic(this, &UAbilityBase::OnPlayerDiscardCards);
			}
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

	CurrentCooldown = FMath::Clamp(NewCooldown, 0, Cooldown);
	bReady = CurrentCooldown == 0;
	OnAbilityCooldownChanged.Broadcast(AbilitySlotIndex, CurrentCooldown);
}

void UAbilityBase::SetAvailability(bool InAvailability)
{
	bAvailable = InAvailability;

	UE_LOG(LogInit, Log, TEXT("[Ability] Set %s availability to %s"), *AbilityName.ToString(), *FString(InAvailability ? TEXT("true") : TEXT("false")));
	OnAbilityAvailabilityChanged.Broadcast(AbilitySlotIndex, bAvailable);
}

bool UAbilityBase::IsAbilityUsable() const
{
	return bAvailable && bReady;
}

bool UAbilityBase::IsAbilityAvailable(EAbilityAvailablePhase CurrentPhase, bool bOwnerRound) const
{
	const bool OwnerMatched = (TEST_SINGLE_FLAG(AbilityAvailablePhase, EAbilityAvailablePhase::AAP_OwnerPhase) && bOwnerRound)
		|| (TEST_SINGLE_FLAG(AbilityAvailablePhase, EAbilityAvailablePhase::AAP_OtherPhase) && !bOwnerRound);
	return TEST_SINGLE_FLAG(AbilityAvailablePhase, CurrentPhase) && OwnerMatched;
}

void UAbilityBase::OnPrePlayerRoundBegin(int32 PlayerId)
{
	SetAvailability(IsAbilityAvailable(EAbilityAvailablePhase::AAP_PrePlayerRoundBegin, PlayerId == OwnerPlayerId));
}

void UAbilityBase::OnPlayerRoundBegin(int32 PlayerId)
{
	SetAvailability(IsAbilityAvailable(EAbilityAvailablePhase::AAP_DuringPlayerRound, PlayerId == OwnerPlayerId));
}

void UAbilityBase::OnPlayerDiscardCards(int32 PlayerId)
{
	SetAvailability(IsAbilityAvailable(EAbilityAvailablePhase::AAP_DiscardCard, PlayerId == OwnerPlayerId));
}

void UAbilityBase::OnPlayerRoundEnd(int32 PlayerId)
{
	if (OwnerPlayerId == PlayerId)
	{
		SetCooldown(CurrentCooldown - 1);
	}
}
