// Dog Fight Game Code By CYM.


#include "Subsystem/GameplayDataSubsystem.h"

#include "Common/GameExperienceDef.h"
#include "Engine/AssetManager.h"
#include "GameMode/DogFightGameModeBase.h"

UGameplayDataSubsystem::UGameplayDataSubsystem()
{
	bGameExperienceLoaded = false;
}

void UGameplayDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadAllGameExperiences();
}

TArray<UGameplayExperience*> UGameplayDataSubsystem::GetGameplayExperiencesByType(EGameplayExperienceType::Type InType)
{
	TArray<UGameplayExperience*> Result;
	if (auto AssetManager = UAssetManager::GetIfValid())
	{
		TArray<FAssetData> AssetDataArray;
		if (AssetManager->GetPrimaryAssetDataList(NAME_GAME_EXPERIENCE, AssetDataArray))
		{
			for (auto& AssetData : AssetDataArray)
			{
				if (auto Experience = Cast<UGameplayExperience>(AssetData.GetAsset()))
				{
					if (Experience->GameType == InType)
					{
						Result.AddUnique(Experience);
					}
				}
			}
		}
	}
	return Result;
}

void UGameplayDataSubsystem::LoadGameplayExperience(UGameplayExperience* GameplayExperience)
{
	if (!IsValid(GameplayExperience))
	{
		return;
	}

	// Notify client the loading
	if (auto GameMode = GetWorld()->GetAuthGameMode())
	{
		if (auto DogFightGM = Cast<ADogFightGameModeBase>(GameMode))
		{
			DogFightGM->NotifyClientGameWillStart();
		}
	}

	FString TravelUrl = GameplayExperience->GetMapURL();
	TravelUrl += "?listen";
	GetWorld()->ServerTravel(TravelUrl, true);
}

void UGameplayDataSubsystem::LoadAllGameExperiences()
{
	if (auto AssetManager = UAssetManager::GetIfValid())
	{
		AssetManager->LoadPrimaryAssetsWithType(NAME_GAME_EXPERIENCE, TArray<FName>(), FStreamableDelegate::CreateLambda([&]()
		{
			bGameExperienceLoaded = true;
		}));
	}
}
