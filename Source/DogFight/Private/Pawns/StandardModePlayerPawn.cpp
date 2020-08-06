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

	PlayerInputComponent->BindAxis("MoveForward", this, &AStandardModePlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStandardModePlayerPawn::MoveRight);
}

UStandardModeCameraComponent* AStandardModePlayerPawn::GetStandardModeCameraComponent() const
{
	check(StandardModeCameraComponent != nullptr);
	return StandardModeCameraComponent;
}
