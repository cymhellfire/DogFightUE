#include "AssetFactory/AssetType/WarheadAssetTypeActions.h"

#include "GameObject/Warhead/WarheadBase.h"

UClass* FWarheadAssetTypeActions::GetSupportedClass() const
{
	return UWarheadBase::StaticClass();
}

FText FWarheadAssetTypeActions::GetName() const
{
	return INVTEXT("Warhead Data");
}

FColor FWarheadAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FWarheadAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}
