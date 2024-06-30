#include "ActionGameEditor.h"

#include "AssetToolsModule.h"
#include "AssetFactories/AssetTypeActions/AssetTypeActions_AvatarActionData.h"
#include "AssetFactories/AssetTypeActions/AssetTypeActions_WeaponActionData.h"
#include "AssetFactories/AssetTypeActions/AssetTypeActions_TargetingWeaponActionData.h"

#define LOCTEXT_NAMESPACE "FActionGameEditorModule"

void FActionGameEditorModule::StartupModule()
{
	auto& AssetToolModule = FAssetToolsModule::GetModule().Get();
	WeaponActionDataAssetTypeAction = MakeShared<FAssetTypeActions_WeaponActionData>();
	AssetToolModule.RegisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
	AvatarActionDataAssetTypeAction = MakeShared<FAssetTypeActions_AvatarActionData>();
	AssetToolModule.RegisterAssetTypeActions(AvatarActionDataAssetTypeAction.ToSharedRef());
	TargetingWeaponActionDataAssetTypeAction = MakeShared<FAssetTypeActions_TargetingWeaponActionData>();
	AssetToolModule.RegisterAssetTypeActions(TargetingWeaponActionDataAssetTypeAction.ToSharedRef());
}

void FActionGameEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		auto& AssetToolModule = FAssetToolsModule::GetModule().Get();
		AssetToolModule.UnregisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
		AssetToolModule.UnregisterAssetTypeActions(AvatarActionDataAssetTypeAction.ToSharedRef());
		AssetToolModule.UnregisterAssetTypeActions(TargetingWeaponActionDataAssetTypeAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FActionGameEditorModule, ActionGameEditor)