// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
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
 * Data asset that describe a character appearance.
 */
UCLASS(Blueprintable)
class ACTIONGAME_API UAvatarDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="AvatarDataAsset")
	FAvatarDescData AvatarDescData;
};
