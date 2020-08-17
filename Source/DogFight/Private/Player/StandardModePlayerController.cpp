// Dog Fight Game Code By CYM.


#include "StandardModePlayerController.h"

#include "StandardModePlayerPawn.h"
#include "StandardModeCameraComponent.h"
#include "DogFight.h"
#include "StandardModePlayerCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"

AStandardModePlayerController::AStandardModePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bShowMouseCursor = true;
	bMoveToMouseCursor = false;
	CharacterPawn = nullptr;
	PrimaryActorTick.bCanEverTick = true;
}

void AStandardModePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keep updating the destination every tick
	if (bMoveToMouseCursor)
	{
		// Update cursor position
		GetMouseCursorPosition();

		// Only do the movement on server side
		if (GetLocalRole() == ROLE_Authority)
		{
			MoveToMouseCursor();
		}
	}
}

void AStandardModePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SpawnCharacterPawn();
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

void AStandardModePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps ) const
{
	DOREPLIFETIME(AStandardModePlayerController, CharacterPawnClass);
	DOREPLIFETIME(AStandardModePlayerController, CharacterPawn);
	DOREPLIFETIME(AStandardModePlayerController, bMoveToMouseCursor);
}

void AStandardModePlayerController::MoveToMouseCursor() const
{
	if (CharacterPawn == nullptr)
		return;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterPawn->GetController(), CursorWorldPosition);
}

void AStandardModePlayerController::GetMouseCursorPosition()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (HitResult.bBlockingHit)
	{
		float const Distance = FVector::Dist(HitResult.Location, CharacterPawn->GetActorLocation());
		if (Distance > 120.f)
		{
			CmdUploadMouseCursorPosition(HitResult.Location);
		}
	}
}

void AStandardModePlayerController::CmdUploadMouseCursorPosition_Implementation(FVector Position)
{
	CursorWorldPosition = Position;
}

void AStandardModePlayerController::OnSetDestinationPressed()
{
	// Call RPC function on server side
	CmdStartMovement();
}

void AStandardModePlayerController::OnSetDestinationReleased()
{
	// Call RPC function on server side
	CmdStopMovement();
}

void AStandardModePlayerController::CmdStartMovement_Implementation()
{
	bMoveToMouseCursor = true;
}

void AStandardModePlayerController::CmdStopMovement_Implementation()
{
	bMoveToMouseCursor = false;
}

void AStandardModePlayerController::SpawnCharacterPawn_Implementation()
{
	// Only do spawn on server side
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	UE_LOG(LogDogFight, Display, TEXT("%s Request Spawn Character Pawn."), *GetName());
	
	// Spawn character pawn now
	if (CharacterPawn == nullptr)
	{
		if (CharacterPawnClass != nullptr)
		{
			// Get a random PlayerStart as spawn location
			TArray<AActor*> StartPoints;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPoints);
			const FVector StartPoint = StartPoints.Num() > 0 ? StartPoints[FMath::RandRange(0, StartPoints.Num() - 1)]->GetActorLocation() : FVector::ZeroVector;

			CharacterPawn = GetWorld()->SpawnActor<AStandardModePlayerCharacter>(
				CharacterPawnClass->GetDefaultObject()->GetClass(), StartPoint,
				RootComponent->GetComponentRotation());
			CharacterPawn->SetOwner(this);
			UE_LOG(LogDogFight, Display, TEXT("Spawn location at %s"), *(StartPoint.ToString()));
		}
		else
		{
			UE_LOG(LogDogFight, Error, TEXT("No pawn has been specified as character."));
		}
	}
}



