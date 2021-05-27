// Dog Fight Game Code By CYM.


#include "Ability/PermanentBuffAbility.h"
#include "Actors/Buffs/BuffBase.h"
#include "Game/StandardGameMode.h"
#include "Pawns/StandardModePlayerCharacter.h"

void UPermanentBuffAbility::Activate()
{
	Super::Activate();

	if (IsValid(BuffClass))
	{
		BuffInstance = GetWorld()->SpawnActor<ABuffBase>(BuffClass);

		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			AStandardModePlayerCharacter* Character = StandardGameMode->GetPlayerCharacterById(OwnerPlayerId);
			BuffInstance->SetTargetActor(Character);
			BuffInstance->SetSourcePlayerController(Character->GetSupremeController());
			BuffInstance->SetLifetime(0);
		}
	}
}


void UPermanentBuffAbility::Deactivate()
{
	// Destroy buff actor before deactivate
	if (IsValid(BuffInstance))
	{
		BuffInstance->EndBuff();
		BuffInstance = nullptr;
	}

	Super::Deactivate();
}
