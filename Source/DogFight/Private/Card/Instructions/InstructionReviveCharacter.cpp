// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionReviveCharacter.h"

#include "Pawns/StandardModePlayerCharacter.h"
#include "Player/StandardModePlayerController.h"

bool UInstructionReviveCharacter::HandleActorTarget(AActor* Target)
{
	if (!Super::HandleActorTarget(Target))
	{
		return false;
	}

	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Target))
	{
		if (AStandardModePlayerController* StandardModePlayerController = Cast<AStandardModePlayerController>(StandardModePlayerCharacter->GetSupremeController()))
		{
			StandardModePlayerController->ReviveCharacter();
		}
	}

	return false;
}
