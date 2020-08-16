// Dog Fight Game Code By CYM.


#include "StandardModePlayerPawnMovement.h"

#include "StandardModePlayerPawn.h"
#include "StandardModeCameraComponent.h"
#include "StandardModePlayerController.h"
#include "DogFight/DogFight.h"

UStandardModePlayerPawnMovement::UStandardModePlayerPawnMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxSpeed = 5000.f;
	Acceleration = 5000.f;
	Deceleration = 5000.f;
}

void UStandardModePlayerPawnMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	const AStandardModePlayerPawn* PlayerPawn = Cast<AStandardModePlayerPawn>(GetOwner());
	const AStandardModePlayerController* PC = Cast<AStandardModePlayerController>(PlayerPawn->GetController());
	if (PC == nullptr)
	{
		return;
	}

	if (PlayerPawn != nullptr)
	{
		FVector MyLocation = UpdatedComponent->GetComponentLocation();
		if (PlayerPawn != nullptr && PlayerPawn->GetStandardModeCameraComponent() != nullptr)
		{
			PlayerPawn->GetStandardModeCameraComponent()->ClampCameraLocation(PC, MyLocation);
		}
		UpdatedComponent->SetWorldLocation(MyLocation);
	}
}
