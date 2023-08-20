// Dog Fight Game Code By CYM.


#include "Controller/ActionCharacterAIController.h"

#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


// Sets default values
AActionCharacterAIController::AActionCharacterAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AActionCharacterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (ControllerState)
	{
	case EDogFightAIControllerState::DFACS_Moving:
		TrackStopDistance();
		break;
	case EDogFightAIControllerState::DFACS_None:
	default: ;
	}
}

void AActionCharacterAIController::MoveToTargetWithStopDistance(const FVector& Target, float InStopDistance)
{
	StopDistance = InStopDistance;
	MoveTarget = Target;

	ControllerState = EDogFightAIControllerState::DFACS_Moving;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Target);
}

void AActionCharacterAIController::TrackStopDistance()
{
	const auto CurrentPath = UAIBlueprintHelperLibrary::GetCurrentPath(this);
	float LeftPathLength = CurrentPath ? CurrentPath->GetPathLength() : 0.f;
	if (LeftPathLength <= StopDistance)
	{
		// Exit moving state once reach the stop distance
		OnReachStopDistance.Broadcast();

		ControllerState = EDogFightAIControllerState::DFACS_None;
	}
}
