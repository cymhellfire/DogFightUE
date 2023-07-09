// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Common/GameExperienceDef.h"
#include "Engine/DataAsset.h"
#include "GameplayExperience.generated.h"

UENUM(BlueprintType)
namespace EGameplayExperienceType
{
	enum Type
	{
		None,
		GameLobby,
		Skirmish,
	};
}

/**
 * 
 */
UCLASS(Blueprintable)
class DOGFIGHTGAME_API UGameplayExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FString GetMapURL() const;

public:
	/** The specified map to load. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=GameplayExperience, meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapId;

	/** Title of this experience. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=GameplayExperience)
	FText Title;

	/** Description of this experience. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=GameplayExperience)
	FText Desc;

	/** Game type of this experience. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=GameplayExperience)
	TEnumAsByte<EGameplayExperienceType::Type> GameType;

	/** Preview image of this experience. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=GameplayExperience)
	UTexture2D* MapPreview;
};
