#include "Pawn/PlayerPawn/Component/TopDownStyleMovementComponent.h"
#include "Pawn/PlayerPawn/TopDownStylePlayerPawn.h"
#include "Pawn/PlayerPawn/Component/TopDownStyleCameraComponent.h"

UTopDownStyleMovementComponent::UTopDownStyleMovementComponent()
{
	MaxSpeed = 5000;
	Acceleration = 5000;
	Deceleration = 5000;
}

void UTopDownStyleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	const ATopDownStylePlayerPawn* PlayerPawn = Cast<ATopDownStylePlayerPawn>(GetOwner());
	const APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController());
	if (PC == nullptr)
	{
		return;
	}

	if (PlayerPawn != nullptr)
	{
		UTopDownStyleCameraComponent* CameraComponent = PlayerPawn->GetCameraComponent();
		FVector MyLocation = UpdatedComponent->GetComponentLocation();
		if (PlayerPawn != nullptr && CameraComponent != nullptr)
		{
			CameraComponent->ClampPawnLocation(PC, MyLocation);
		}
		UpdatedComponent->SetWorldLocation(MyLocation);
	}
}
