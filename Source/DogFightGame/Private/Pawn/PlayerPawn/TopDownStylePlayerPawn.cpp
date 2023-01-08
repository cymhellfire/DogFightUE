#include "Pawn/PlayerPawn/TopDownStylePlayerPawn.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Pawn/PlayerCharacter/FreeForAllPlayerCharacter.h"
#include "Pawn/PlayerPawn/Component/TopDownStyleCameraComponent.h"
#include "Pawn/PlayerPawn/Component/TopDownStyleMovementComponent.h"
#include "PlayerController/TopDownStylePlayerController.h"

ATopDownStylePlayerPawn::ATopDownStylePlayerPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTopDownStyleMovementComponent>(MovementComponentName))
{
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;

	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	// Create camera component
	CameraComponent = CreateDefaultSubobject<UTopDownStyleCameraComponent>("CameraComponent");
}

void ATopDownStylePlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement input
	PlayerInputComponent->BindAxis("MoveForward", this, &ATopDownStylePlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATopDownStylePlayerPawn::MoveRight);

	// Character movement input
	PlayerInputComponent->BindAction("SetDestination", IE_Released, this, &ATopDownStylePlayerPawn::OnSetDestination);
}

void ATopDownStylePlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParam;
	SharedParam.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerPawn, bEnableCharacterMove, SharedParam);
}

void ATopDownStylePlayerPawn::OnSetDestination()
{
	if (!bEnableCharacterMove)
	{
		return;
	}

	if (auto PC = Cast<APlayerController>(GetController()))
	{
		FHitResult HitResult;
		if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			ServerMoveCharacter(HitResult.Location);
		}
	}
}

void ATopDownStylePlayerPawn::ServerMoveCharacter_Implementation(const FVector& Destination)
{
	if (auto PC = Cast<ATopDownStylePlayerController>(GetController()))
	{
		if (auto Character = PC->GetCharacterPawn())
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(Character->GetController(), Destination);
		}
	}
}

void ATopDownStylePlayerPawn::ServerSetCharacterMovable_Implementation(bool bEnable)
{
	if (bEnableCharacterMove == bEnable)
	{
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerPawn, bEnableCharacterMove, this);
	bEnableCharacterMove = bEnable;
}
