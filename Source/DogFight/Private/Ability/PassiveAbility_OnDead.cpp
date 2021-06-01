// Dog Fight Game Code By CYM.


#include "Ability/PassiveAbility_OnDead.h"

#include "Game/StandardGameMode.h"
#include "Pawns/StandardModePlayerCharacter.h"

bool UPassiveAbility_OnDead::IsAbilityUsable() const
{
	return bReady;
}

void UPassiveAbility_OnDead::RegisterCondition()
{
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		AStandardModePlayerCharacter* StandardModePlayerCharacter = StandardGameMode->GetPlayerCharacterById(OwnerPlayerId);
		if (IsValid(StandardModePlayerCharacter))
		{
			StandardModePlayerCharacter->OnCharacterDead.AddDynamic(this, &UPassiveAbility_OnDead::OnCharacterDead);
		}
	}
}

void UPassiveAbility_OnDead::UnregisterCondition()
{
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		AStandardModePlayerCharacter* StandardModePlayerCharacter = StandardGameMode->GetPlayerCharacterById(OwnerPlayerId);
		if (IsValid(StandardModePlayerCharacter))
		{
			StandardModePlayerCharacter->OnCharacterDead.RemoveDynamic(this, &UPassiveAbility_OnDead::OnCharacterDead);
		}
	}
}

void UPassiveAbility_OnDead::OnCharacterDead()
{
	Activate();
}
