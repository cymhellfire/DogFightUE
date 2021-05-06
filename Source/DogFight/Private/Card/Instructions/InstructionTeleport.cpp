// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionTeleport.h"

#include "Actors/Components/ActorTeleportComponent.h"
#include "BlueprintLibrary/DogFightGameplayLibrary.h"
#include "AI/StandardModeAIController.h"
#include "Card/CardBase.h"
#include "Player/StandardModePlayerController.h"

UInstructionTeleport::UInstructionTeleport(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoFinish = false;
}

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

	if (UActorTeleportComponent* TeleportComponent = UDogFightGameplayLibrary::TeleportActor(UsingCardPawn, Position))
	{
		TeleportComponent->OnTeleportFinished.AddDynamic(this, &UInstructionTeleport::OnTeleportFinished);
	}
	else
	{
		Finish();
	}
}

void UInstructionTeleport::OnTeleportFinished(UActorTeleportComponent* Component)
{
	Component->OnTeleportFinished.RemoveDynamic(this, &UInstructionTeleport::OnTeleportFinished);

	Finish();
}
