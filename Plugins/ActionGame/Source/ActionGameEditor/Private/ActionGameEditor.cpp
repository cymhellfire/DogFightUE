#include "..\Public\ActionGameEditor.h"

#include "AssetToolsModule.h"
#include "AssetFactories/AssetTypeActions/WeaponActionDataAssetTypeAction.h"

#define LOCTEXT_NAMESPACE "FActionGameEditorModule"

void FActionGameEditorModule::StartupModule()
{
	auto& AssetToolModule = FAssetToolsModule::GetModule().Get();
	WeaponActionDataAssetTypeAction = MakeShared<FWeaponActionDataAssetTypeAction>();
	AssetToolModule.RegisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
}

void FActionGameEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		auto& AssetToolModule = FAssetToolsModule::GetModule().Get();
		AssetToolModule.UnregisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FActionGameEditorModule, ActionGameEditor)