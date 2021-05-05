// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionTeleport.h"

#include "BlueprintLibrary/DogFightGameplayLibrary.h"
#include "AI/StandardModeAIController.h"
#include "Card/CardBase.h"
#include "Player/StandardModePlayerController.h"

void UInstructionTeleport::HandlePositionTarget(FVector Position)
{
	Super::HandlePositionTarget(Position);

	APawn* UsingCardPawn = nullptr;
	if (AStandardModePlayerController* StandardModePlayerController = Cast<AStandardModePlayerController>(GetOwnerCard()->GetOwnerPlayerController()))
	{
		UsingCardPawn = StandardModePlayerController->GetActualPawn();
	}
	else if (AStandardModeAIController* StandardModeAIController = Cast<AStandardModeAIController>(GetOwnerCard()->GetOwnerPlayerController()))
	{
		UsingCardPawn = StandardModeAIController->GetActualPawn();
	}

	UDogFightGameplayLibrary::TeleportActor(UsingCardPawn, Position);
}
