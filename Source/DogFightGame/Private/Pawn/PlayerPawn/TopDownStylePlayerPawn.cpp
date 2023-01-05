#include "Pawn/PlayerPawn/TopDownStylePlayerPawn.h"

#include "Components/SphereComponent.h"
#include "Pawn/PlayerPawn/Component/TopDownStyleCameraComponent.h"
#include "Pawn/PlayerPawn/Component/TopDownStyleMovementComponent.h"

ATopDownStylePlayerPawn::ATopDownStylePlayerPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTopDownStyleMovementComponent>(MovementComponentName))
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	// Create camera component
	CameraComponent = CreateDefaultSubobject<UTopDownStyleCameraComponent>("CameraComponent");
}

void ATopDownStylePlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATopDownStylePlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATopDownStylePlayerPawn::MoveRight);
}
