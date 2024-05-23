// Dog Fight Game Code By CYM.


#include "Pawn/ActionGameCharacter.h"

#include "Common/ActionGameWeaponLog.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


// Sets default values
AActionGameCharacter::AActionGameCharacter()
{
}

// Called when the game starts or when spawned
void AActionGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
		}
		else
		{
			UE_LOG(LogActionGame, Error, TEXT("[ActionGameCharacter] Invalid animation blueprint specified."))
		}
	}
}
