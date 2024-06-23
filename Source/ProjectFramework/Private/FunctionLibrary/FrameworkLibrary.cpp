// Dog Fight Game Code By CYM.


#include "FunctionLibrary/FrameworkLibrary.h"

#include "Engine/AssetManager.h"
#include "GameInstance/DogFightGameInstance.h"
#include "Kismet/GameplayStatics.h"

FText UFrameworkLibrary::GetTextFromLocalizedString(const FLocalizedString& LocalizedString)
{
	return LocalizedString.GetLocalizeText();
}

void UFrameworkLibrary::LoadGameMap(const UObject* WorldContextObject, const FString& MapPath)
{
	auto& StreamableManager = UAssetManager::Get().GetStreamableManager();

	FSoftObjectPath MapPathObject(MapPath);

	TWeakObjectPtr WeakWorldContext = const_cast<UObject*>(WorldContextObject);
	StreamableManager.RequestAsyncLoad(MapPathObject, FStreamableDelegate::CreateLambda([WeakWorldContext, MapPathObject]()
	{
		if (WeakWorldContext.IsValid())
		{
			UGameplayStatics::OpenLevel(WeakWorldContext->GetWorld(), MapPathObject.GetAssetPath().GetAssetName());
		}
	}));
}

bool UFrameworkLibrary::IsCurrentMap(const UObject* WorldContextObject, const FString& MapPath)
{
	if (auto CurWorld = WorldContextObject->GetWorld())
	{
		if (auto CurrentLevel = CurWorld->GetCurrentLevel())
		{
			if (UPackage* Package = CurrentLevel->GetOutermost())
			{
				auto PackagePath = CurWorld->RemovePIEPrefix(Package->GetName());
				return PackagePath == MapPath;
			}
		}
	}

	return false;
}
