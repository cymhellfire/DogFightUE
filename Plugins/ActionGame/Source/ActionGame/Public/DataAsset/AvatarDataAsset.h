// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Common/ActionAnimCommon.h"
#include "Engine/DataAsset.h"
#include "AvatarDataAsset.generated.h"

/**
 * Avatar appearance data.
 */
USTRUCT(BlueprintType)
struct FAvatarDescData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="AvatarDescData")
	FText AvatarName;

	UPROPERTY(EditAnywhere, Category="AvatarDescData", meta=(AllowedClasses="/Script/Engine.SkeletalMesh"))
	FSoftObjectPath SkeletalMeshAsset;

	UPROPERTY(EditAnywhere, Category="AvatarDescData")
	FSoftClassPath AnimationBlueprintClass;

	bool IsValid() const
	{
		return SkeletalMeshAsset.IsAsset() && AnimationBlueprintClass.IsAsset();
	}
};

/**
 * Avatar animation data.
 */
USTRUCT(BlueprintType)
struct FAvatarAnimSetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="AvatarAnimSetData")
	TMap<TEnumAsByte<EActionAnimPredefinedType::Type>, UAnimMontage*> AnimResourceMap;
};

USTRUCT(BlueprintType)
struct FAvatarPhysicsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="AvatarPhysicsData")
	float CollisionRadius = 34.f;

	UPROPERTY(EditAnywhere, Category="AvatarPhysicsData")
	float CollisionHalfHeight = 88.f;

	/** Additional offset for skeletal mesh component after applied negative CollisionHalfHeight. */
	UPROPERTY(EditAnywhere, Category="AvatarPhysicsData")
	float SkeletalMeshOffset = -2.f;
};

/**
 * Data asset that describe a character appearance.
 */
UCLASS(Blueprintable)
class ACTIONGAME_API UAvatarDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="AvatarDataAsset")
	FAvatarDescData AvatarDescData;

	UPROPERTY(EditAnywhere, Category="AvatarDataAsset")
	FAvatarAnimSetData AvatarAnimSetData;

	UPROPERTY(EditAnywhere, Category="AvatarDataAsset")
	FAvatarPhysicsData AvatarPhysicsData;
};
