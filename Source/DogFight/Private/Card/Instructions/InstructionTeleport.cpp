// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionTeleport.h"

#include "Actors/Components/ActorTeleportComponent.h"
#include "BlueprintLibrary/DogFightGameplayLibrary.h"
#include "AI/StandardModeAIController.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"
#include "GameFramework/PlayerState.h"
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
		TeleportComponent->OnArrivedDestination.AddDynamic(this, &UInstructionTeleport::OnCharacterTeleported);
		TeleportComponent->OnTeleportFinished.AddDynamic(this, &UInstructionTeleport::OnTeleportFinished);
	}
	else
	{
		Finish();
	}
}

void UInstructionTeleport::OnCharacterTeleported(UActorTeleportComponent* Component)
{
	Component->OnArrivedDestination.RemoveDynamic(this, &UInstructionTeleport::OnCharacterTeleported);

	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (APlayerState* OwnerPlayerState = GetOwnerCard()->GetOwnerPlayerController()->GetPlayerState<APlayerState>())
		{
			const FVector CurrentLoc = Component->GetOwner()->GetActorLocation();
			StandardGameMode->BroadcastCameraFocusEvent(
				FCameraFocusEvent
				{
					OwnerPlayerState->GetPlayerId(),
					CurrentLoc.X,
					CurrentLoc.Y,
					ECameraFocusEventType::Type::OwnerForced
				});
		}
	}
}

void UInstructionTeleport::OnTeleportFinished(UActorTeleportComponent* Component)
{
	Component->OnTeleportFinished.RemoveDynamic(this, &UInstructionTeleport::OnTeleportFinished);

	Finish();
}
