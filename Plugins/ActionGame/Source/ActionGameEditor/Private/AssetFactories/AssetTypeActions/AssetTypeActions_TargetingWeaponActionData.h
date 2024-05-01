#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "DataAsset/TargetingWeaponActionDataAsset.h"

class FAssetTypeActions_TargetingWeaponActionData : public FAssetTypeActions_Base
{
public:
	UClass* GetSupportedClass() const override
	{
		return UTargetingWeaponActionDataAsset::StaticClass();
	}

	virtual FText GetName() const override
	{
		return INVTEXT("New Targeting Weapon Action Data");
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
