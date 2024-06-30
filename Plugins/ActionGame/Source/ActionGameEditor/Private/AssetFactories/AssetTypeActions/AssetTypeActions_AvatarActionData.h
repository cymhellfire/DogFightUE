#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "DataAsset/AvatarDataAsset.h"

class FAssetTypeActions_AvatarActionData : public FAssetTypeActions_Base
{
public:
	UClass* GetSupportedClass() const override
	{
		return UAvatarDataAsset::StaticClass();
	}

	virtual FText GetName() const override
	{
		return INVTEXT("New Avatar Data");
	}

	virtual FColor GetTypeColor() const override
	{
		return FColor::Blue;
	}

	virtual uint32 GetCategories() override
	{
		return EAssetTypeCategories::Gameplay;
	}
};
