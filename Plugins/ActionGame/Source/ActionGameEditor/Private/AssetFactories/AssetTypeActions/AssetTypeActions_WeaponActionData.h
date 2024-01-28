#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "DataAsset/WeaponActionDataAsset.h"

class FAssetTypeActions_WeaponActionData : public FAssetTypeActions_Base
{
public:
	UClass* GetSupportedClass() const override
	{
		return UWeaponActionDataAsset::StaticClass();
	}

	virtual FText GetName() const override
	{
		return INVTEXT("New Weapon Action Data");
	}

	virtual FColor GetTypeColor() const override
	{
		return FColor::Cyan;
	}

	virtual uint32 GetCategories() override
	{
		return EAssetTypeCategories::Gameplay;
	}
};
