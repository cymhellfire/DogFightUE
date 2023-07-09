// Dog Fight Game Code By CYM.


#include "DataAsset/GameplayExperience.h"

#include "Engine/AssetManager.h"

FString UGameplayExperience::GetMapURL() const
{
	if (MapId.IsValid())
	{
		FAssetData AssetData;
		if (UAssetManager::Get().GetPrimaryAssetData(MapId, AssetData))
		{
			return AssetData.PackageName.ToString();
		}
	}

	return "";
}
