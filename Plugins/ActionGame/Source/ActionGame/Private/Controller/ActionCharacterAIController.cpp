// Dog Fight Game Code By CYM.


#include "Controller/ActionCharacterAIController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"


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
	SqrStopDistance = InStopDistance * InStopDistance;
	MoveTarget = Target;

	SetState(EDogFightAIControllerState::DFACS_Moving);
}

void AActionCharacterAIController::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	// Skip if this caused by manually stop to prevent from infinity loop.
	if (bManualStop)
	{
		return;
	}

	SetState(EDogFightAIControllerState::DFACS_None);
}

void AActionCharacterAIController::SetState(EDogFightAIControllerState NewState)
{
	if (ControllerState == NewState)
	{
		return;
	}

	ControllerState = NewState;
	switch(NewState)
	{
	case EDogFightAIControllerState::DFACS_None:
		{
			TGuardValue<bool> ManualStopGuard(bManualStop, true);
			StopMovement();
		}
		break;
	case EDogFightAIControllerState::DFACS_Moving:
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MoveTarget);
			if (auto PFComp = GetPathFollowingComponent())
			{
				if (!PFComp->OnRequestFinished.IsBoundToObject(this))
				{
					PFComp->OnRequestFinished.AddUObject(this, &AActionCharacterAIController::OnMoveRequestFinished);
				}
			}
		}
		break;
	default: ;
	}
}

void AActionCharacterAIController::TrackStopDistance()
{
	FVector ActorLoc = FVector::ZeroVector;
	bool bValid = false;
	if (auto MyPawn = GetPawn())
	{
		ActorLoc = MyPawn->GetActorLocation();
		bValid = true;
	}
	DrawDebugLine(GetWorld(), ActorLoc, MoveTarget, FColor::Yellow);

	float SqrDistance = bValid ? (MoveTarget - ActorLoc).SizeSquared() : 0.f;
	if (SqrDistance <= SqrStopDistance)
	{
		// Exit moving state once reach the stop distance
		OnReachStopDistance.Broadcast();

		SetState(EDogFightAIControllerState::DFACS_None);
	}
}
