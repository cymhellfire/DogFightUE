// Dog Fight Game Code By CYM.


#include "StandardModePlayerPawn.h"

#include "StandardModePlayerPawnMovement.h"
#include "StandardModeCameraComponent.h"
#include "DogFight/DogFight.h"

AStandardModePlayerPawn::AStandardModePlayerPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UStandardModePlayerPawnMovement>(Super::MovementComponentName))
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	StandardModeCameraComponent = CreateDefaultSubobject<UStandardModeCameraComponent>(TEXT("StandardCameraComponent"));
}

void AStandardModePlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("CameraDraggingMove", EInputEvent::IE_Pressed, this, &AStandardModePlayerPawn::OnDraggingMovementStart);
	PlayerInputComponent->BindAction("CameraDraggingMove", EInputEvent::IE_Released, this, &AStandardModePlayerPawn::OnDraggingMovementStop);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStandardModePlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStandardModePlayerPawn::MoveRight);
}

void AStandardModePlayerPawn::OnDraggingMovementStart()
{
	check(StandardModeCameraComponent != nullptr);
	StandardModeCameraComponent->StartDraggingMovement();
}

void AStandardModePlayerPawn::OnDraggingMovementStop()
{
	check(StandardModeCameraComponent != nullptr);
	StandardModeCameraComponent->StopDraggingMovement();
}

UStandardModeCameraComponent* AStandardModePlayerPawn::GetStandardModeCameraComponent() const
{
	check(StandardModeCameraComponent != nullptr);
	return StandardModeCameraComponent;
}
