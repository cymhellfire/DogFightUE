#include "Ability/UsingCardAbility.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"

bool UUsingCardAbility::Activate()
{
	if (!Super::Activate())
		return false;

	// Spawn and use card if specified
	if (IsValid(AbilityCardClass))
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			ACardBase* NewCard = GetWorld()->SpawnActor<ACardBase>(AbilityCardClass);
			NewCard->SetOwnerPlayerController(StandardGameMode->GetControllerById(OwnerPlayerId));
			NewCard->Use();

			return true;
		}
	}

	return false;
}

