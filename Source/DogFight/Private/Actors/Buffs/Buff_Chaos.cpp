// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_Chaos.h"

#include "Actors/Interfaces/GameTargetProviderInterface.h"
#include "Pawns/StandardModePlayerCharacter.h"

void ABuff_Chaos::ApplyBuff()
{
	Super::ApplyBuff();

	// Get target controller
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		if (IGameTargetProviderInterface* TargetProvider = Cast<IGameTargetProviderInterface>(StandardModePlayerCharacter->GetSupremeController()))
		{
			OnTargetActorSelectedHandle = TargetProvider->GetOnTargetActorSelectedDelegate().AddUObject(this, &ABuff_Chaos::OnTargetActorSelected);
		}
	}
}

void ABuff_Chaos::RemoveBuff()
{
	// Get target controller
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		if (IGameTargetProviderInterface* TargetProvider = Cast<IGameTargetProviderInterface>(StandardModePlayerCharacter->GetSupremeController()))
		{
			TargetProvider->GetOnTargetActorSelectedDelegate().Remove(OnTargetActorSelectedHandle);
		}
	}

	Super::RemoveBuff();
}

void ABuff_Chaos::OnTargetActorSelected(AActor** SelectedActor)
{
	const float TestNumber = FMath::RandRange(0.f, 1.f);
	if (TestNumber <= ChaosChance)
	{
		// Replace actor with current target
		*SelectedActor = TargetActor;

		// Add floating text here
		if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
		{
			PlayerCharacter->MulticastAddFloatingText(ChaosTriggerText.GetLocalizeText());
		}
	}
}
