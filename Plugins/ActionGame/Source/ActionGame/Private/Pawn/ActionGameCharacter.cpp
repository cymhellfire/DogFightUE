// Dog Fight Game Code By CYM.


#include "Pawn/ActionGameCharacter.h"

#include "Common/ActionGameWeaponLog.h"
#include "Controller/ActionCharacterAIController.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Pawn/Component/CharacterAnimComponent.h"


// Sets default values
AActionGameCharacter::AActionGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AnimComponent = CreateDefaultSubobject<UCharacterAnimComponent>("CharacterAnimComponent");

	// Set default controller
	AIControllerClass = AActionCharacterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::Spawned;
}

// Called when the game starts or when spawned
void AActionGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (auto AIController = Cast<AActionCharacterAIController>(GetController()))
	{
		if (auto PathFollowingComponent = AIController->GetPathFollowingComponent())
		{
			PathFollowingComponent->OnRequestFinished.AddUObject(this, &AActionGameCharacter::OnMoveFinished);
		}

		AIController->OnReachStopDistance.AddUObject(this, &AActionGameCharacter::OnReachStopDistance);
	}
}

void AActionGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (auto AIController = Cast<AActionCharacterAIController>(GetController()))
	{
		if (auto PathFollowingComponent = AIController->GetPathFollowingComponent())
		{
			PathFollowingComponent->OnRequestFinished.RemoveAll(this);
		}

		AIController->OnReachStopDistance.RemoveAll(this);
	}
}

void AActionGameCharacter::SetupAvatarAppearance(const FAvatarDescData& AvatarDescData)
{
	if (!AvatarDescData.IsValid())
	{
		UE_LOG(LogActionGame, Error, TEXT("[ActionGameCharacter] Invalid avatar data asset."))
		return;
	}

	// Check if any unload asset
	TArray<FSoftObjectPath> AssetToLoad;
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(AvatarDescData.SkeletalMeshAsset.ResolveObject());
	if (!IsValid(SkeletalMesh))
	{
		AssetToLoad.Add(AvatarDescData.SkeletalMeshAsset);
	}
	UAnimBlueprintGeneratedClass* AnimBlueprint = Cast<UAnimBlueprintGeneratedClass>(AvatarDescData.AnimationBlueprintClass.ResolveClass());
	if (!IsValid(AnimBlueprint))
	{
		AssetToLoad.Add(AvatarDescData.AnimationBlueprintClass);
	}

	if (AssetToLoad.Num() > 0)
	{
		// Load necessary assets
		auto& StreamableManager = UAssetManager::GetStreamableManager();
		// Generate a Guid to identify load task
		AsyncLoadGuid = FGuid::NewGuid();
		auto GuidStr = AsyncLoadGuid.ToString();
		StreamableManager.RequestAsyncLoad(AssetToLoad, FStreamableDelegate::CreateLambda([AvatarDescData, GuidStr, this]()
		{
			if (AsyncLoadGuid.ToString() == GuidStr)
			{
				// Apply after loaded
				ApplyAvatarDesc(AvatarDescData);
			}
		}), FStreamableManager::AsyncLoadHighPriority);
	}
	else
	{
		ApplyAvatarDesc(AvatarDescData);
	}
}

void AActionGameCharacter::SetupAvatarAppearanceWithAsset(UAvatarDataAsset* InAsset)
{
	if (IsValid(InAsset))
	{
		SetupAvatarAppearance(InAsset->AvatarDescData);
		ApplyAnimationSet(InAsset->AvatarAnimSetData);
	}
	else
	{
		UE_LOG(LogActionGame, Error, TEXT("[ActionGameCharacter] Invalid avatar data asset."))
	}
}

void AActionGameCharacter::ApplyAvatarDesc(const FAvatarDescData& AvatarDescData)
{
	UE_LOG(LogActionGame, Log, TEXT("[ActionGameCharacter] Apply avatar desc data: %s"), *AvatarDescData.AvatarName.ToString())

	if (auto SkelMeshComp = GetMesh())
	{
		if (auto NewSkeletalMesh = Cast<USkeletalMesh>(AvatarDescData.SkeletalMeshAsset.ResolveObject()))
		{
			SkelMeshComp->SetSkeletalMeshAsset(NewSkeletalMesh);
		}
		else
		{
			UE_LOG(LogActionGame, Error, TEXT("[ActionGameCharacter] Invalid skeletal mesh specified."))
		}
		if (auto NewAnimBp = Cast<UAnimBlueprintGeneratedClass>(AvatarDescData.AnimationBlueprintClass.ResolveClass()))
		{
			SkelMeshComp->SetAnimInstanceClass(NewAnimBp);

			// Notify animation component to update
			if (IsValid(AnimComponent))
			{
				AnimComponent->RefreshAnimInstance();
			}
		}
		else
		{
			UE_LOG(LogActionGame, Error, TEXT("[ActionGameCharacter] Invalid animation blueprint specified."))
		}
	}
}

void AActionGameCharacter::ApplyAnimationSet(const FAvatarAnimSetData& AvatarAnimSetData)
{
	if (IsValid(AnimComponent))
	{
		AnimComponent->SetupPredefineAnimations(AvatarAnimSetData);
	}
}

float AActionGameCharacter::PlayActionAnimation(UAnimMontage* InMontage)
{
	if (IsValid(AnimComponent))
	{
		return AnimComponent->PlayAnimation(InMontage);
	}
	return IActionCharacterInterface::PlayActionAnimation(InMontage);
}

float AActionGameCharacter::PlayActionAnimationWithWarping(UAnimMontage* InMontage, FName TargetName,
	const FVector& TargetPos)
{
	if (IsValid(AnimComponent))
	{
		return AnimComponent->PlayAnimationWithWarping(InMontage, TargetName, TargetPos);
	}
	return IActionCharacterInterface::PlayActionAnimationWithWarping(InMontage, TargetName, TargetPos);
}

float AActionGameCharacter::PlayPredefineAnimation(EActionAnimPredefinedType::Type InType)
{
	if (IsValid(AnimComponent))
	{
		return AnimComponent->PlayPredefineAnimation(InType);
	}
	return IActionCharacterInterface::PlayPredefineAnimation(InType);
}

float AActionGameCharacter::PlayPredefineAnimationWithWarping(EActionAnimPredefinedType::Type InType, FName TargetName,
	const FVector& TargetPos)
{
	if (IsValid(AnimComponent))
	{
		return AnimComponent->PlayPredefineAnimationWithWarping(InType, TargetName, TargetPos);
	}
	return IActionCharacterInterface::PlayPredefineAnimationWithWarping(InType, TargetName, TargetPos);
}

void AActionGameCharacter::MoveToTarget(const FVector& Target, float StopDistance)
{
	if (auto AIController = Cast<AActionCharacterAIController>(GetController()))
	{
		AIController->MoveToTargetWithStopDistance(Target, StopDistance);
	}
}

void AActionGameCharacter::StopMoveImmediately()
{
	if (auto MovementComponent = GetMovementComponent())
	{
		MovementComponent->StopMovementImmediately();
	}
}

void AActionGameCharacter::OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		OnCharacterMoveFinished.Broadcast();
	}
}

void AActionGameCharacter::OnReachStopDistance()
{
	StopMoveImmediately();
	OnReachActionDistance.Broadcast();
}
