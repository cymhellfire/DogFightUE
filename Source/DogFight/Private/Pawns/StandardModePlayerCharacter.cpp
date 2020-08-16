// Dog Fight Game Code By CYM.


#include "StandardModePlayerCharacter.h"

#include "DogFight.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "StandardGameState.h"
#include "StandardModePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AStandardModePlayerCharacter::AStandardModePlayerCharacter()
{
	bReplicates = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Do not rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;	// Make sure character face to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.f, 0.f, 0.f).Quaternion());

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void AStandardModePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStandardModePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update decal location
	if (CursorToWorld != nullptr)
	{
		AStandardModePlayerController const * const PC = Cast<AStandardModePlayerController>(GetOwner());
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			UWorld* World = GetWorld();
			if (World != nullptr && PC != nullptr)
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = PC->PlayerCameraManager->GetCameraLocation();
				FVector EndLocation = PC->PlayerCameraManager->GetCameraRotation().Vector() * 2000.f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (PC != nullptr)
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocationAndRotation(TraceHitResult.Location, CursorR);
		}
	}
}

// Called to bind functionality to input
void AStandardModePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

