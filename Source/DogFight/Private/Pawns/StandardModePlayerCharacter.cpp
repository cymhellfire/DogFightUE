// Dog Fight Game Code By CYM.


#include "Pawns/StandardModePlayerCharacter.h"

#include "DogFight.h"
#include "GameMode/DogFightGameModeBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "NavigationPath.h"
#include "Actors/Projectiles/ProjectileBase.h"
#include "Player/StandardModePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Actors/Components/ReceiveDamageComponent.h"
#include "Actors/Components/ActorTeleportComponent.h"
#include "Actors/Weapons/WeaponBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Game/StandardPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Game/StandardGameMode.h"
#include "Actors/Weapons/WeaponDisplayRelative.h"
#include "Actors/Weapons/WeaponActionBase.h"
#include "Pawns/TestAttributeComponent.h"
#include "UI/Widget/CharacterFloatingTextPanelWidget.h"

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
	CharacterStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("CharacterStatusWidgetComponent");
	CharacterStatusWidgetComponent->SetupAttachment(RootComponent);
	CharacterStatusWidgetComponent->SetDrawSize(FVector2D(150.f, 30.f));
	CharacterStatusWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	CharacterStatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	// Create floating text component
	FloatingTextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("FloatingTextWidgetComponent");
	FloatingTextWidgetComponent->SetupAttachment(RootComponent);
	FloatingTextWidgetComponent->SetDrawSize(FVector2D(500.f, 250.f));
	FloatingTextWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingTextWidgetComponent->SetPivot(FVector2D(0.5f, 1.f));

	// Create ReceiveDamageComponent
	ReceiveDamageComponent = CreateDefaultSubobject<UReceiveDamageComponent>("ReceiveDamageComponent");
	ReceiveDamageComponent->SetIsReplicated(true);

	// Create TeleportComponent
	TeleportComponent = CreateDefaultSubobject<UActorTeleportComponent>("TeleportComponent");
	TeleportComponent->SetIsReplicated(true);

	// Create AttributeComponent
	AttributeComponent = CreateDefaultSubobject<UTestAttributeComponent>("AttributeComponent");

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Initial value
	MaxBaseHealth = 100;
	MaxStrength = 50;
	bShowCursorToWorld = false;
	AimingState = 0;
	AimingApproximateAngle = 1.f;
	AimingRotateSpeed = 90.f;
	bAlive = true;

	FollowBoneName = FName(TEXT("pelvis"));
	RagdollFloorDetectHeight = 100.f;
	bRagdoll = false;
	PoseSlotName = FName(TEXT("RagdollFinalPose"));
	RagdollAutoRecoverDelay = 2.f;
	RagdollStopThreshold = 1.5;

	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Actor.Character"))));
}

void AStandardModePlayerCharacter::SetSupremeController(AController* NewController)
{
	check(NewController);

	SupremeController = NewController;
}

void AStandardModePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardModePlayerCharacter, UnitName);
	DOREPLIFETIME(AStandardModePlayerCharacter, CurrentHealth);
	DOREPLIFETIME(AStandardModePlayerCharacter, MaxBaseHealth);
	DOREPLIFETIME(AStandardModePlayerCharacter, CurrentStrength);
	DOREPLIFETIME(AStandardModePlayerCharacter, MaxStrength);
	DOREPLIFETIME(AStandardModePlayerCharacter, CacheBlastForce);
	DOREPLIFETIME(AStandardModePlayerCharacter, CurrentWeaponType);
}

void AStandardModePlayerCharacter::OnRep_SyncRagdollRotation()
{
	if (bRagdoll)
	{
		if (USkeletalMeshComponent* MeshComponent = GetMesh())
		{
			MeshComponent->SetRelativeRotation(SyncRagdollRotation);
		}
	}
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

void AStandardModePlayerCharacter::SetCurrentHealth(float NewHealth)
{
	if (GetLocalRole() != ROLE_Authority)
		return;
	
	const float NewValue = FMath::Clamp<float>(NewHealth, 0.f, MaxBaseHealth);
	if (CurrentHealth != NewValue)
	{
		CurrentHealth = NewValue;
		OnRep_CurrentHealth();
	}
}

void AStandardModePlayerCharacter::SetMaxHealth(float NewMaxHealth)
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	const int32 CeilMaxHealth = FMath::CeilToInt(NewMaxHealth);
	const int32 DeltaHealth = CeilMaxHealth - MaxBaseHealth;
	MaxBaseHealth = CeilMaxHealth;
	// Also modify current health
	SetCurrentHealth(FMath::Clamp<int32>(CurrentHealth + DeltaHealth, bAlive ? 1 : 0, MaxBaseHealth));

	MaxHealthChanged(MaxBaseHealth);
}

void AStandardModePlayerCharacter::SetCurrentStrength(float NewStrength)
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	const float NewValue = FMath::Clamp<float>(NewStrength, 0.f, MaxStrength);
	if (CurrentStrength != NewValue)
	{
		CurrentStrength = NewValue;
		OnRep_CurrentStrength();
	}
}

void AStandardModePlayerCharacter::SetMaxStrength(float NewMaxStrength)
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	const int32 CeilMaxStrength = FMath::CeilToInt(NewMaxStrength);
	const int32 DeltaStrength = CeilMaxStrength - MaxStrength;
	MaxStrength = CeilMaxStrength;
	// Also modify current strength
	SetCurrentStrength(FMath::Clamp<int32>(CurrentStrength + DeltaStrength, bRagdoll ? 0 : 1, MaxStrength));

	MaxStrengthChanged(MaxStrength);
}

UReceiveDamageComponent* AStandardModePlayerCharacter::GetDamageReceiveComponent()
{
	return ReceiveDamageComponent;
}

void AStandardModePlayerCharacter::AddInvincibleFlags(int32 Flags)
{
	if (ReceiveDamageComponent != nullptr)
	{
		ReceiveDamageComponent->AddInvincibleFlags(Flags);
	}
}

void AStandardModePlayerCharacter::RemoveInvincibleFlags(int32 Flags)
{
	if (ReceiveDamageComponent != nullptr)
	{
		ReceiveDamageComponent->RemoveInvincibleFlags(Flags);
	}
}

bool AStandardModePlayerCharacter::AddExtraArmor(FActorArmor& NewArmor)
{
	if (ReceiveDamageComponent != nullptr)
	{
		return ReceiveDamageComponent->AddExtraArmor(NewArmor);
	}

	return false;
}

bool AStandardModePlayerCharacter::RemoveExtraArmor(FActorArmor& TargetArmor)
{
	if (ReceiveDamageComponent != nullptr)
	{
		return ReceiveDamageComponent->RemoveExtraArmor(TargetArmor);
	}

	return false;
}

void AStandardModePlayerCharacter::SetHealthPercentage(float NewPercentage)
{
	SetCurrentHealth(MaxBaseHealth * NewPercentage);
}

void AStandardModePlayerCharacter::ApplyDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Handle damage
	TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

float AStandardModePlayerCharacter::PlayMontage(UAnimMontage* MontageToPlay)
{
	// Play montage across network
	MulticastPlayMontage(MontageToPlay);

	return IsValid(MontageToPlay) ? MontageToPlay->GetPlayLength() : 0.f;
}

UBuffQueue* AStandardModePlayerCharacter::GetBuffQueue()
{
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		return StandardPlayerState->GetBuffQueue();
	}

	return nullptr;
}

void AStandardModePlayerCharacter::GetGameplayTags(FGameplayTagContainer& OutGameplayTags)
{
	OutGameplayTags = GameplayTags;
}

void AStandardModePlayerCharacter::MulticastPlayMontage_Implementation(UAnimMontage* MontageToPlay)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float Result = AnimInstance->Montage_Play(MontageToPlay);
		if (Result == 0)
		{
			UE_LOG(LogAnimation, Error, TEXT("Failed to play montage."));
		}
	}
}

EWeaponType AStandardModePlayerCharacter::GetCurrentWeaponType()
{
	return CurrentWeaponType;
}

FWeaponActionDisplayInfo AStandardModePlayerCharacter::GetNextActionDisplayInfoByInput(EWeaponActionInput Input) const
{
	if (IsValid(CurrentWeapon))
	{
		return CurrentWeapon->GetNextActionByInput(Input)->GetWeaponActionDisplayInfo();
	}

	return FWeaponActionDisplayInfo();
}

void AStandardModePlayerCharacter::EquipWeapon(UWeaponBase* NewWeapon)
{
	if (NewWeapon == CurrentWeapon)
	{
		return;
	}

	// UnEquip previous weapon first
	if (IsValid(CurrentWeapon))
	{
		PendingWeapon = NewWeapon;
		UnEquipWeapon();
	}
	else
	{
		CurrentWeapon = NewWeapon;
		CurrentWeaponType = CurrentWeapon->GetWeaponType();
		CurrentWeapon->SetWeaponOwner(this);
		CurrentWeapon->OnWeaponEquippedEvent.AddDynamic(this, &AStandardModePlayerCharacter::OnWeaponEquipped);
		CurrentWeapon->OnWeaponActionFinishedEvent.AddDynamic(this, &AStandardModePlayerCharacter::OnWeaponActionFinished);
		CurrentWeapon->Equip();
	}
}

void AStandardModePlayerCharacter::UnEquipWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	CurrentWeapon->OnWeaponActionFinishedEvent.RemoveDynamic(this, &AStandardModePlayerCharacter::OnWeaponActionFinished);
	CurrentWeapon->OnWeaponUnEquippedEvent.AddDynamic(this, &AStandardModePlayerCharacter::OnWeaponUnEquipped);
	CurrentWeapon->UnEquip();
	CurrentWeaponType = EWeaponType::WT_None;
}

void AStandardModePlayerCharacter::OnWeaponEquipped()
{
	CurrentWeapon->OnWeaponEquippedEvent.RemoveDynamic(this, &AStandardModePlayerCharacter::OnWeaponEquipped);

	// Trigger callback
	OnWeaponEquippedEvent.Broadcast(this);
}

void AStandardModePlayerCharacter::OnWeaponUnEquipped()
{
	CurrentWeapon->OnWeaponUnEquippedEvent.RemoveDynamic(this, &AStandardModePlayerCharacter::OnWeaponUnEquipped);
	CurrentWeapon = nullptr;

	if (IsValid(PendingWeapon))
	{
		EquipWeapon(PendingWeapon);
		PendingWeapon = nullptr;
	}
}

void AStandardModePlayerCharacter::OnWeaponActionFinished()
{
	// Broadcast callback
	OnWeaponActionFinishedEvent.Broadcast(this);
}

void AStandardModePlayerCharacter::OnPlayerRoundBegin(int32 PlayerId)
{
	// Cache current location when round begin
	CacheCurrentLocation();
}

void AStandardModePlayerCharacter::OnPlayerRoundEnd(int32 PlayerId)
{
	if (APlayerState* MyPlayerState = GetPlayerState())
	{
		if (MyPlayerState->GetPlayerId() == PlayerId)
		{
			// Clear weapon target and reset weapon action to initial one
			if (CurrentWeapon != nullptr)
			{
				WeaponTargetActor = nullptr;
				CurrentWeapon->ResetWeaponAction();
			}
		}
	}
}

void AStandardModePlayerCharacter::OnTeleportFinished(UActorTeleportComponent* Component)
{
	// Update cache location after teleport
	CacheCurrentLocation();
}

void AStandardModePlayerCharacter::EnqueueInput(EWeaponActionInput NewInput)
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	CurrentWeapon->EnqueueWeaponInput(NewInput);
}

void AStandardModePlayerCharacter::SetWeaponTargetActor(AActor* NewTarget)
{
	WeaponTargetActor = NewTarget;
}

void AStandardModePlayerCharacter::SetWeaponTargetLocation(FVector NewLocation)
{
	WeaponTargetLocation = NewLocation;
}

void AStandardModePlayerCharacter::MoveToActionDistance()
{
	bTracingActionDistance = true;
	if (WeaponTargetActor)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(this->GetController(), WeaponTargetActor);
	}
	else
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), WeaponTargetLocation);
	}
}

void AStandardModePlayerCharacter::SetActionDistance(float NewDistance)
{
	WeaponActionDistance = NewDistance;
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

		CurrentStrength = MaxStrength;
		OnRep_CurrentStrength();
	}

	// Setup the physical animation component
	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
	SkeletalMeshOffset = SkeletalMeshComponent->GetRelativeLocation();

	// Check the floating text widget
	FloatingTextPanelWidget = Cast<UCharacterFloatingTextPanelWidget>(FloatingTextWidgetComponent->GetWidget());
	if (!FloatingTextPanelWidget)
	{
		UE_LOG(LogInit, Log, TEXT("No widget available"));
	}

	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameMode))
		{
			StandardGameMode->OnPlayerRoundBegin.AddDynamic(this, &AStandardModePlayerCharacter::OnPlayerRoundBegin);
			StandardGameMode->OnPlayerRoundEnd.AddDynamic(this, &AStandardModePlayerCharacter::OnPlayerRoundEnd);
		}
	}

	TeleportComponent->OnTeleportFinished.AddDynamic(this, &AStandardModePlayerCharacter::OnTeleportFinished);
}

void AStandardModePlayerCharacter::OnRep_UnitName()
{
	UnitNameChanged(FText::FromString(UnitName));
}

void AStandardModePlayerCharacter::OnRep_MaxBaseHealth()
{
	MaxHealthChanged(MaxBaseHealth);
}

void AStandardModePlayerCharacter::OnRep_CurrentHealth()
{
	if (CurrentHealth <= 0.f)
	{
		Dead();
	}

	// Use ceil version of health for other logic
	const int32 CeilHealth = GetCurrentHealth();

	// Invoke delegate
	OnCharacterHealthChanged.Broadcast(CeilHealth);

	// Invoke Blueprint implementation
	CurrentHealthChanged(CeilHealth);
}

void AStandardModePlayerCharacter::OnRep_MaxStrength()
{
	MaxStrengthChanged(MaxStrength);
}

void AStandardModePlayerCharacter::OnRep_CurrentStrength()
{
	// Skip the ragdoll setup if player is dead
	if (!bAlive)
		return;

	if (CurrentStrength <= 0)
	{
		bRagdollAutoRecover = true;
		SetRagdollActive(true);
	}

	// Invoke delegate
	OnCharacterStrengthChanged.Broadcast(CurrentStrength);

	// Invoke Blueprint implementation
	CurrentStrengthChanged(CurrentStrength);
}

void AStandardModePlayerCharacter::Dead()
{
	bAlive = false;

	OnCharacterDead.Broadcast();

	// Enable physical animation
	bRagdollAutoRecover = false;
	MulticastSetRagdollActive(true);
	if (RagdollAutoRecoverTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RagdollAutoRecoverTimerHandle);
	}
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

			// Invoke callback
			if (FacingFinishCallback)
			{
				FacingFinishCallback();
			}
		}
		SetActorRotation(CurrentRotation);
	}

	// Synchronize capsule and target bone position
	if (bRagdoll)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		FVector BonePosition = SkeletalMeshComponent->GetBoneLocation(FollowBoneName);
		if (GetNetMode() != NM_Client)
		{
			FHitResult TraceResult;
			FVector FloorDetectOffset(0, 0, RagdollFloorDetectHeight);
			if (GetWorld()->LineTraceSingleByChannel(TraceResult, BonePosition,
				BonePosition - FloorDetectOffset, ECollisionChannel::ECC_Visibility))
			{
				GetCapsuleComponent()->SetWorldLocation(TraceResult.Location - SkeletalMeshOffset);

				//DrawDebugLine(GetWorld(), BonePosition + FloorDetectOffset, TraceResult.Location, FColor::Yellow);
			}
			else
			{
				GetCapsuleComponent()->SetWorldLocation(BonePosition);
			}

			// Synchronize rotation
			SyncRagdollRotation = SkeletalMeshComponent->GetRelativeRotation().Quaternion();
		}
		else
		{
			// Synchronize ragdoll to capsule on client side
			SkeletalMeshComponent->SetRelativeLocation(SkeletalMeshOffset);
		}
	}

	if (bTracingActionDistance)
	{
		const UNavigationPath* CurrentPath = UAIBlueprintHelperLibrary::GetCurrentPath(this->GetController());
		float LeftPathLength = CurrentPath ? CurrentPath->GetPathLength() : 0.f;
		if (LeftPathLength <= WeaponActionDistance)
		{
			bTracingActionDistance = false;
			StopMoveImmediately();

			// Trigger callback
			OnCarrierReachedActionDistanceEvent.Broadcast(this);
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
		CurrentHealth = FMath::Clamp<float>(CurrentHealth - ActualDamage, 0, MaxBaseHealth);

		// Invoke OnRep on server side manually
		OnRep_CurrentHealth();

		// Test code for attribute
		if (AttributeComponent)
		{
			AttributeComponent->ChangeTestAttribute(CurrentHealth);
		}

		// Show damage
		// Check damage type and format the text
		FString DamageText = FString::Printf(TEXT("%.2f"), ActualDamage);
		if (UDogFightDamageType* DogFightDamage = Cast<UDogFightDamageType>(DamageEvent.DamageTypeClass.GetDefaultObject()))
		{
			const FString StyleName = DogFightDamage->DamageStyleName;
			if (!StyleName.IsEmpty())
			{
				DamageText = FString::Printf(TEXT("<img id=\"%s\"/><%s>%s</>"), *DogFightDamage->DamageTypeName.LocalizeKey, *StyleName, *DamageText);
			}
		}

		MulticastAddFloatingText(FText::FromString(DamageText));
	}

	return ActualDamage;
}

void AStandardModePlayerCharacter::TakeStrengthCost(float StrengthCost, FVector KnockdownForce)
{
	CurrentStrength = FMath::Clamp<int32>(CurrentStrength - StrengthCost, 0, MaxStrength);

	// Cache blast force value
	if (CurrentStrength <= 0)
	{
		CacheBlastForce = KnockdownForce;
	}

	UE_LOG(LogInit, Log, TEXT("Calculated CacheBlastForce: %s"), *CacheBlastForce.ToString());

	// Invoke OnRep on server
	OnRep_CurrentStrength();
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

void AStandardModePlayerCharacter::SetAimingDirection(FVector NewDirection, TFunction<void()> Callback)
{
	// Decide rotating clockwise or not
	DesireFacingRotation = NewDirection.Rotation();
	float YawDelta = DesireFacingRotation.Yaw - GetActorRotation().Yaw;

	// Filter little direction changes
	if (FMath::Abs(YawDelta) < 1.f)
	{
		// Invoke callback
		if (Callback)
		{
			Callback();
		}
		return;
	}

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

	FacingFinishCallback = Callback;
}

void AStandardModePlayerCharacter::RecoverStrength()
{
	if (GetNetMode() != NM_Client)
	{
		CurrentStrength = MaxStrength;

		OnRep_CurrentStrength();
	}
}

void AStandardModePlayerCharacter::Revive()
{
	// Skip if character is not dead yet
	if (bAlive)
		return;

	// Recover health point and strength
	SetHealthPercentage(1);

	// Change alive flag after character stand up
	//bAlive = true;

	// Turn off ragdoll
	MulticastSetRagdollActive(false);

	// Broadcast the reviving state start
	OnCharacterReviveStateChanged.Broadcast(this, true);
}

void AStandardModePlayerCharacter::MulticastAddFloatingText_Implementation(const FText& NewText)
{
	ShowFloatingText(NewText);
}

void AStandardModePlayerCharacter::CacheCurrentLocation()
{
	CachedLocation = GetActorLocation();
}

void AStandardModePlayerCharacter::ReturnToCachedLocation()
{
	SetWeaponTargetLocation(CachedLocation);
	ClearWeaponTargetActor();
	MoveToActionDistance();
}

void AStandardModePlayerCharacter::EquipTestWeapon()
{
	if (IsValid(TestWeaponClass))
	{
		UWeaponBase* NewWeapon = NewObject<UWeaponBase>(this, TestWeaponClass);
		EquipWeapon(NewWeapon);
	}
}

void AStandardModePlayerCharacter::ShowFloatingText(FText NewText)
{
	if (FloatingTextPanelWidget != nullptr)
	{
		FloatingTextPanelWidget->AddFloatText(NewText);
	}
}

#pragma region Ragdoll
void AStandardModePlayerCharacter::MulticastSetRagdollActive_Implementation(bool bActive)
{
	SetRagdollActive(bActive);
}

void AStandardModePlayerCharacter::SetRagdollActive(bool bActive)
{
	if (bActive)
	{
		bRagdoll = true;
		// Disable capsule collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Enable Physical Animation
		USkeletalMeshComponent* MeshComponent = GetMesh();
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetConstraintProfileForAll(FName(TEXT("ragdoll")));

		// Apply cached blast force
		MeshComponent->AddImpulseToAllBodiesBelow(CacheBlastForce, PelvisBoneName);

		K2_OnRagdollEnabled();

		// Server only logic
		if (GetNetMode() != NM_Client)
		{
			// Start auto recover tick if necessary
			if (bRagdollAutoRecover)
			{
				RagdollRecoverTimer = 0.f;
				GetWorldTimerManager().SetTimer(RagdollAutoRecoverTimerHandle, this, &AStandardModePlayerCharacter::RagdollAutoRecoverTick, 0.1f, true);
			}

			// Synchronize to PlayerState
			if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->SetRagdollActive(true);
			}
		}

		OnCharacterRagdollStateChanged.Broadcast(this, bActive);
	}
	else
	{
		PreCacheRagdollPose();

		// Recover all strength after disable ragdoll
		RecoverStrength();

		// Re-cache location that character will leave here
		CacheCurrentLocation();
	}
}

void AStandardModePlayerCharacter::AddForceToAllRagdollBodies(FVector Force)
{
	// Skip if it's not Ragdoll mode currently
	if (!bRagdoll)
	{
		return;
	}

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->AddForceToAllBodiesBelow(Force, PelvisBoneName);
}

void AStandardModePlayerCharacter::PreCacheRagdollPose()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	const FQuat BoneRot = MeshComponent->GetBoneQuaternion(FName(TEXT("pelvis")));
	const float DirNum = FVector::DotProduct(BoneRot.GetRightVector(), FVector::UpVector);
	bRagdollFacingUp = DirNum > 0.f;

	SynchronizeOrientationWithRagdoll(bRagdollFacingUp);
	GetWorldTimerManager().SetTimer(RagdollRecoverTimerHandle, this, &AStandardModePlayerCharacter::DoCacheRagdollPose, 0.03f);
}

void AStandardModePlayerCharacter::DoCacheRagdollPose()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->SavePoseSnapshot(PoseSlotName);
	}

	// Invoke Blueprint event
	K2_OnCacheRagdollPose();

	GetWorldTimerManager().SetTimer(RagdollRecoverTimerHandle, this, &AStandardModePlayerCharacter::PostCacheRagdollPose, 0.2f);
}

void AStandardModePlayerCharacter::PostCacheRagdollPose()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetSimulatePhysics(false);
	UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	float Result = AnimInstance->Montage_Play(bRagdollFacingUp ? GetUpFromFaceMontage : GetUpFromBackMontage);
	if (Result == 0.f)
	{
		UE_LOG(LogActor, Error, TEXT("Failed to play get up animation montage."));
	}
	else
	{
		GetWorldTimerManager().SetTimer(RagdollRecoverTimerHandle, this, &AStandardModePlayerCharacter::OnCharacterGetUp, Result);
	}

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Pawn")));
	bRagdoll = false;

	// Synchronize to PlayerState
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		StandardPlayerState->SetRagdollActive(false);
	}

	// Invoke Blueprint event
	K2_OnPostCacheRagdollPose();

	OnCharacterRagdollStateChanged.Broadcast(this, false);
}

void AStandardModePlayerCharacter::OnCharacterGetUp()
{
	if (!bAlive)
	{
		// If character stand up when bAlive equals false, it must be revived
		bAlive = true;

		// Recover strength
		RecoverStrength();

		// Broadcast the reviving state end
		OnCharacterReviveStateChanged.Broadcast(this, false);
	}
}

void AStandardModePlayerCharacter::RagdollAutoRecoverTick()
{
	float CurrentSpeed = GetMesh()->GetPhysicsLinearVelocity(PelvisBoneName).Size();

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("Ragdoll speed: %f"), CurrentSpeed));

	if (CurrentSpeed <= RagdollStopThreshold)
	{
		RagdollRecoverTimer += 0.1f;
	}
	else
	{
		RagdollRecoverTimer = 0.f;
	}

	// Disable ragdoll if timer is expired
	if (RagdollRecoverTimer >= RagdollAutoRecoverDelay)
	{
		GetWorldTimerManager().ClearTimer(RagdollAutoRecoverTimerHandle);
		MulticastSetRagdollActive(false);
	}
}

void AStandardModePlayerCharacter::SynchronizeOrientationWithRagdoll(bool bIsFacingUp)
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	FVector HeadBoneLoc = MeshComponent->GetBoneLocation(NeckBoneName);
	FVector PelvisBoneLoc = MeshComponent->GetBoneLocation(PelvisBoneName);
	FVector Direction = bIsFacingUp ? PelvisBoneLoc - HeadBoneLoc : HeadBoneLoc - PelvisBoneLoc;
	FRotator NewRotator = UKismetMathLibrary::MakeRotFromZX(FVector::UpVector, Direction);
	GetCapsuleComponent()->SetWorldRotation(NewRotator);
}
#pragma endregion Ragdoll
