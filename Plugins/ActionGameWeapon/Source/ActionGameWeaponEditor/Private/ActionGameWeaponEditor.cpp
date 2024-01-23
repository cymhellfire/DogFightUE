#include "ActionGameWeaponEditor.h"

#include "AssetToolsModule.h"
#include "AssetFactories/AssetTypeActions/WeaponActionDataAssetTypeAction.h"

#define LOCTEXT_NAMESPACE "FActionGameWeaponEditorModule"

void FActionGameWeaponEditorModule::StartupModule()
{
	auto& AssetToolModule = FAssetToolsModule::GetModule().Get();
	WeaponActionDataAssetTypeAction = MakeShared<FWeaponActionDataAssetTypeAction>();
	AssetToolModule.RegisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
}

void FActionGameWeaponEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		auto& AssetToolModule = FAssetToolsModule::GetModule().Get();
		AssetToolModule.UnregisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FActionGameWeaponEditorModule, ActionGameWeaponEditor)