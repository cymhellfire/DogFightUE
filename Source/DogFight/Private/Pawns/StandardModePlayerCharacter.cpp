// Dog Fight Game Code By CYM.


#include "StandardModePlayerCharacter.h"

#include "DogFight.h"
#include "DogFightGameModeBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "StandardGameState.h"
#include "StandardModePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "ReceiveDamageComponent.h"

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

	// Create ReceiveDamageComponent
	ReceiveDamageComponent = CreateDefaultSubobject<UReceiveDamageComponent>("ReceiveDamageComponent");
	ReceiveDamageComponent->SetIsReplicated(true);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Initial value
	MaxBaseHealth = 100;
	bShowCursorToWorld = false;
	AimingState = 0;
	AimingApproximateAngle = 1.f;
	AimingRotateSpeed = 90.f;
	bAlive = true;
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

UReceiveDamageComponent* AStandardModePlayerCharacter::GetDamageReceiveComponent()
{
	return ReceiveDamageComponent;
}

void AStandardModePlayerCharacter::SetPhysicalArmor(int32 NewArmor)
{
	if (ReceiveDamageComponent != nullptr)
	{
		ReceiveDamageComponent->PhysicalArmor = NewArmor;
	}
}

int32 AStandardModePlayerCharacter::GetPhysicalArmor() const
{
	if (ReceiveDamageComponent != nullptr)
	{
		return ReceiveDamageComponent->PhysicalArmor;
	}

	return 0;
}

void AStandardModePlayerCharacter::SetMagicalArmor(int32 NewArmor)
{
	if (ReceiveDamageComponent != nullptr)
	{
		ReceiveDamageComponent->MagicalArmor = NewArmor;
	}
}

int32 AStandardModePlayerCharacter::GetMagicalArmor() const
{
	if (ReceiveDamageComponent != nullptr)
	{
		return ReceiveDamageComponent->MagicalArmor;
	}

	return 0;
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
	if (CurrentHealth <= 0)
	{
		Dead();
	}

	// Invoke Blueprint implementation
	CurrentHealthChanged(CurrentHealth);
}

void AStandardModePlayerCharacter::Dead()
{
	bAlive = false;

	OnCharacterDead.Broadcast();
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

	if (AimingState != 0)
	{
		FRotator CurrentRotation = GetActorRotation();

		// Rotate Clockwise
		if (AimingState == 1)
		{
			CurrentRotation.Yaw += DeltaTime * AimingRotateSpeed;
		}
		// Rotate count Clockwise
		else if (AimingState == 2)
		{
			CurrentRotation.Yaw -= DeltaTime * AimingRotateSpeed;
		}

		if (FMath::Abs(CurrentRotation.Yaw - DesireFacingRotation.Yaw) <= AimingApproximateAngle)
		{
			CurrentRotation.Yaw = DesireFacingRotation.Yaw;
			AimingState = 0;
		}
		SetActorRotation(CurrentRotation);
	}
}

// Called to bind functionality to input
void AStandardModePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AStandardModePlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bAlive)
	{
		return 0.f;
	}

	float ActualDamage = 0;
	// Modify the damage by current GameMode
	if (ADogFightGameModeBase* DogFightGameModeBase = Cast<ADogFightGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		ActualDamage = DogFightGameModeBase->CalculateDamage(this, Damage, DamageEvent, EventInstigator, DamageCauser);
	}

	// Apply modified damage
	ActualDamage = Super::TakeDamage(ActualDamage, DamageEvent, EventInstigator, DamageCauser);

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

void AStandardModePlayerCharacter::StopMoveImmediately()
{
	GetMovementComponent()->StopMovementImmediately();
}

void AStandardModePlayerCharacter::SetAimingDirection(FVector NewDirection)
{
	// Decide rotating clockwise or not
	DesireFacingRotation = NewDirection.Rotation();
	float YawDelta = DesireFacingRotation.Yaw - GetActorRotation().Yaw;

	// Keep the angle in range [0,360)
	if (YawDelta < 0)
		YawDelta += 360;

	if (YawDelta <= 180)
	{
		// Clockwise
		AimingState = 1;
	}
	else
	{
		// Count clockwise
		AimingState = 2;
	}
}

