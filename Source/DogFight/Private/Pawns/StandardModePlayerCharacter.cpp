// Dog Fight Game Code By CYM.


#include "StandardModePlayerCharacter.h"

#include "DogFight.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "StandardGameState.h"
#include "StandardModePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

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
	CursorToWorld->SetVisibility(false);

	// Create the Widget component for status UI display
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetDrawSize(FVector2D(150.f, 30.f));
	WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Initial value
	MaxBaseHealth = 100;
	bShowCursorToWorld = false;
}

void AStandardModePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardModePlayerCharacter, UnitName);
	DOREPLIFETIME(AStandardModePlayerCharacter, CurrentHealth);
}

void AStandardModePlayerCharacter::SetUnitName(const FString& NewName)
{
	// Check if it's necessary to change name
	if (NewName.IsEmpty() || NewName == UnitName)
	{
		return;
	}
	
	UnitName = NewName;

	// Manually invoke OnRep_UnitName on server
	if (GetNetMode() != NM_Client)
	{
		OnRep_UnitName();
	}
}

void AStandardModePlayerCharacter::SetCurrentHealth(int32 NewHealth)
{
	if (GetLocalRole() != ROLE_Authority)
		return;
	
	const int32 NewValue = FMath::Clamp(NewHealth, 0, MaxBaseHealth);
	if (CurrentHealth != NewValue)
	{
		CurrentHealth = NewValue;
		OnRep_CurrentHealth();
	}
}

// Called when the game starts or when spawned
void AStandardModePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set the begin play state
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = MaxBaseHealth;
		OnRep_CurrentHealth();
	}
}

void AStandardModePlayerCharacter::OnRep_UnitName()
{
	UnitNameChanged(FText::FromString(UnitName));
}

void AStandardModePlayerCharacter::OnRep_CurrentHealth()
{
	// Invoke Blueprint implementation
	CurrentHealthChanged(CurrentHealth);
}

// Called every frame
void AStandardModePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update decal location
	if (CursorToWorld != nullptr && bShowCursorToWorld)
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

float AStandardModePlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		CurrentHealth = FMath::Clamp<int32>(CurrentHealth - ActualDamage, 0, MaxBaseHealth);

		// Invoke OnRep on server side manually
		OnRep_CurrentHealth();
	}

	return ActualDamage;
}

void AStandardModePlayerCharacter::SetCursorVisible(bool bVisible)
{
	bShowCursorToWorld = bVisible;

	CursorToWorld->SetVisibility(bVisible);
}

