#include "Pawn/PlayerCharacter/RagdollComponent.h"

#include "Common/DogFightGameLog.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

URagdollComponent::URagdollComponent()
{
	// Enable the initialize component function
	bWantsInitializeComponent = true;
	// Enable tick
	PrimaryComponentTick.bCanEverTick = true;
	// Enable replication
	SetIsReplicatedByDefault(true);
}

void URagdollComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!IsValid(OwnerCharacter))
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[URagdollComponent] Invalid owner type. Expecting ACharacter instance."));
	}
	else
	{
		// Use the capsule component of owner character as default.
		UpdateComponent = OwnerCharacter->GetCapsuleComponent();

		// Record initial offset for skeletal mesh component
		MeshComponentOffset = OwnerCharacter->GetMesh()->GetRelativeLocation();
	}
}

void URagdollComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Synchronize update component and target bone position
	if (bRagdoll && IsValid(OwnerCharacter))
	{
		auto MeshComponent = OwnerCharacter->GetMesh();
		FVector BonePosition = MeshComponent->GetBoneLocation(FollowBoneName);
		if (GetNetMode() != NM_Client)
		{
			FHitResult TraceResult;
			FVector GroundDetectOffset(0, 0, GroundDetectHeight);
			if (GetWorld()->LineTraceSingleByChannel(TraceResult, BonePosition, BonePosition - GroundDetectOffset, ECC_Visibility))
			{
				OwnerCharacter->GetCapsuleComponent()->SetWorldLocation(TraceResult.Location - MeshComponentOffset);
			}
			else
			{
				OwnerCharacter->GetCapsuleComponent()->SetWorldLocation(BonePosition);
			}

			// Synchronize rotation
			MARK_PROPERTY_DIRTY_FROM_NAME(URagdollComponent, SyncRagdollRotation, this);
			SyncRagdollRotation = MeshComponent->GetRelativeRotation().Quaternion();
		}
		else
		{
			// Synchronize ragdoll to capsule on client side
			MeshComponent->SetRelativeLocation(MeshComponentOffset);
		}
	}
}

void URagdollComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(URagdollComponent, SyncRagdollRotation, SharedParams);
}

void URagdollComponent::ServerSetRagdollActive_Implementation(bool bActive)
{
	SetRagdollActive(bActive);
}

void URagdollComponent::SetRagdollActive_Implementation(bool bActive)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (bActive)
	{
		bRagdoll = true;
		// Disable capsule collision
		OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Enable Physical Animation
		USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetConstraintProfileForAll(FName(TEXT("ragdoll")));

		// Trigger callback
		OnRagdollStatusChanged.Broadcast(true);
	}
	else
	{
		PreCacheRagdollPose();
	}
}

void URagdollComponent::PreCacheRagdollPose()
{
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	const FQuat BoneRot = MeshComponent->GetBoneQuaternion(PelvisBoneName);
	const float DirNum = FVector::DotProduct(BoneRot.GetRightVector(), FVector::UpVector);
	bRagdollFacingUp = DirNum > 0.f;

	SynchronizeOrientationWithRagdoll(bRagdollFacingUp);
	OwnerCharacter->GetWorldTimerManager().SetTimer(RagdollRecoverTimerHandle, this, &URagdollComponent::DoCacheRagdollPose, 0.03f);
}

void URagdollComponent::SynchronizeOrientationWithRagdoll(bool bIsFacingUp)
{
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	FVector HeadBoneLoc = MeshComponent->GetBoneLocation(NeckBoneName);
	FVector PelvisBoneLoc = MeshComponent->GetBoneLocation(PelvisBoneName);
	FVector Direction = bIsFacingUp ? PelvisBoneLoc - HeadBoneLoc : HeadBoneLoc - PelvisBoneLoc;
	FRotator NewRotator = UKismetMathLibrary::MakeRotFromZX(FVector::UpVector, Direction);
	OwnerCharacter->GetCapsuleComponent()->SetWorldRotation(NewRotator);
}

void URagdollComponent::DoCacheRagdollPose()
{
	auto MeshComponent = OwnerCharacter->GetMesh();
	auto AnimInstance = MeshComponent->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->SavePoseSnapshot(PoseSlotName);
	}

	// Trigger callback
	OnRagdollCachedPose.Broadcast();

	OwnerCharacter->GetWorldTimerManager().SetTimer(RagdollRecoverTimerHandle, this, &URagdollComponent::PostCacheRagdollPose, 0.2f);
}

void URagdollComponent::PostCacheRagdollPose()
{
	auto MeshComponent = OwnerCharacter->GetMesh();
	MeshComponent->SetSimulatePhysics(false);
	auto AnimInstance = MeshComponent->GetAnimInstance();
	float Result = AnimInstance->Montage_Play(bRagdollFacingUp ? GetUpFromFaceMontage : GetUpFromBackMontage);
	if (Result == 0.f)
	{
		UE_LOG(LogDogFightGame, Error, TEXT("Failed to paly get up animation montage."));
	}
	else
	{
		OwnerCharacter->GetWorldTimerManager().SetTimer(RagdollRecoverTimerHandle, this, &URagdollComponent::OnRecoveredFromRagdoll, Result);
	}

	OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Pawn")));
	bRagdoll = false;

	// Trigger callback
	PostRagdollCachedPose.Broadcast();
	OnRagdollStatusChanged.Broadcast(false);
}

void URagdollComponent::OnRecoveredFromRagdoll()
{
	// Trigger callback
	OnRagdollRecovered.Broadcast();
}

void URagdollComponent::OnRep_SyncRagdollRotation()
{
	if (!bRagdoll)
		return;

	if (auto MeshComponent = OwnerCharacter->GetMesh())
	{
		MeshComponent->SetRelativeRotation(SyncRagdollRotation);
	}
}
