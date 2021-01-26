// Dog Fight Game Code By CYM.


#include "Pawns/StandardModePlayerCharacter.h"

#include "DogFight.h"
#include "Game/DogFightGameModeBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Actors/Projectiles/ProjectileBase.h"
#include "Player/StandardModePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Actors/Components/ReceiveDamageComponent.h"
#include "Game/StandardPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
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

void AStandardModePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardModePlayerCharacter, UnitName);
	DOREPLIFETIME(AStandardModePlayerCharacter, CurrentHealth);
	DOREPLIFETIME(AStandardModePlayerCharacter, CurrentStrength);
	DOREPLIFETIME(AStandardModePlayerCharacter, CacheBlastForce);
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

void AStandardModePlayerCharacter::SetInvincible(bool bActive)
{
	if (ReceiveDamageComponent != nullptr)
	{
		ReceiveDamageComponent->bInvincible = bActive;
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

	// Invoke delegate
	OnCharacterHealthChanged.Broadcast(CurrentHealth);

	// Invoke Blueprint implementation
	CurrentHealthChanged(CurrentHealth);
}

void AStandardModePlayerCharacter::OnRep_CurrentStrength()
{
	if (CurrentStrength <= 0)
	{
		bRagdollAutoRecover = true;
		SetRagdollActive(true);
	}

	// Invoke delegate
	OnCharacterStrengthChanged.Broadcast(CurrentStrength);
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

				DrawDebugLine(GetWorld(), BonePosition + FloorDetectOffset, TraceResult.Location, FColor::Yellow);
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
	
	// Calculate strength cost
	if (UDogFightDamageType* DamageType = Cast<UDogFightDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()))
	{
		CurrentStrength = FMath::Clamp<int32>(CurrentStrength - DamageType->StrengthCost, 0, MaxStrength);

		// Cache blast force value
		if (CurrentStrength <= 0)
		{
			CacheBlastForce = (GetActorLocation() - DamageCauser->GetActorLocation());
			CacheBlastForce.Normalize();
			CacheBlastForce += FVector::UpVector * DamageType->BlastForceUpwardRatio;
			// Calculate the actual force
			if (AProjectileBase* Projectile = Cast<AProjectileBase>(DamageCauser))
			{
				CacheBlastForce *= DamageType->CalculateBlastForceSize(DamageCauser->GetActorLocation(), GetActorLocation(), Projectile->DamageRadius);
			}
			else
			{
				CacheBlastForce *= DamageType->BlastForce;
			}
		}

		UE_LOG(LogInit, Log, TEXT("Calculated CacheBlastForce: %s"), *CacheBlastForce.ToString());

		// Invoke OnRep on server
		OnRep_CurrentStrength();
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

		// Show damage
		FloatingTextPanelWidget->AddDamageText(ActualDamage, DamageEvent.DamageTypeClass.GetDefaultObject());
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

	// Filter little direction changes
	if (FMath::Abs(YawDelta) < 1.f)
	{
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
}

void AStandardModePlayerCharacter::RecoverStrength()
{
	if (GetNetMode() != NM_Client)
	{
		CurrentStrength = MaxStrength;

		OnRep_CurrentStrength();
	}
}

void AStandardModePlayerCharacter::MulticastAddFloatingText_Implementation(const FText& NewText)
{
	ShowFloatingText(NewText);
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
	}
	else
	{
		PreCacheRagdollPose();

		// Recover all strength after disable ragdoll
		RecoverStrength();
	}
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

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Pawn")));
	bRagdoll = false;

	// Synchronize to PlayerState
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		StandardPlayerState->SetRagdollActive(false);
	}

	// Invoke Blueprint event
	K2_OnPostCacheRagdollPose();
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
