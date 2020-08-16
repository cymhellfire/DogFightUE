// Dog Fight Game Code By CYM.


#include "StandardModePlayerController.h"

#include "StandardModePlayerPawn.h"
#include "StandardModeCameraComponent.h"
#include "DogFight.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "StandardModePlayerCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AStandardModePlayerController::AStandardModePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bMoveToMouseCursor = false;
	CharacterPawn = nullptr;
	PrimaryActorTick.bCanEverTick = true;
}

void AStandardModePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Keep updating the destination every tick
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AStandardModePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Spawn character pawn now
	if (CharacterPawn == nullptr)
	{
		if (CharacterPawnClass != nullptr)
		{
			CharacterPawn = GetWorld()->SpawnActor<AStandardModePlayerCharacter>(CharacterPawnClass->GetDefaultObject()->GetClass(), RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
			CharacterPawn->SetOwner(this);
		}
		else
		{
			UE_LOG(LogDogFight, Error, TEXT("No pawn has been specified as character."));
		}
	}
}

void AStandardModePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AStandardModePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AStandardModePlayerController::OnSetDestinationReleased);
}

void AStandardModePlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);

	// Update StandardModePlayerPawn if available
	AStandardModePlayerPawn* CurrentPlayerPawn = Cast<AStandardModePlayerPawn>(GetPawn());
	if (CurrentPlayerPawn != nullptr)
	{
		CurrentPlayerPawn->GetStandardModeCameraComponent()->UpdateCameraMovement(this);
	}
}

void AStandardModePlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (CharacterPawn != nullptr)
		{
			if (CharacterPawn->GetCursorToWorld() != nullptr)
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterPawn->GetController(), CharacterPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		if (HitResult.bBlockingHit)
		{
			float const Distance = FVector::Dist(HitResult.Location, CharacterPawn->GetActorLocation());
			if (Distance > 120.f)
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterPawn->GetController(), HitResult.Location);
			}
		}
	}
}

void AStandardModePlayerController::OnSetDestinationPressed()
{
	bMoveToMouseCursor = true;
}

void AStandardModePlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
}



