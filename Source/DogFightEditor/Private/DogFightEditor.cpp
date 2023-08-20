#include "DogFightEditor.h"

#include "DogFightEditorCommands.h"
#include "DogFightEditorStyle.h"
#include "AssetFactories/AssetTypeActions/WeaponActionDataAssetTypeAction.h"
#include "Toolbars/MainWindowToolbar.h"

#define LOCTEXT_NAMESPACE "FDogFightEditorModule"

void FDogFightEditorModule::StartupModule()
{
	Style = FDogFightEditorStyle::GetInstance();

	FDogFightEditorCommands::Register();

	MainWindowToolbar = MakeShareable(new FMainWindowToolbar);
	MainWindowToolbar->Initialize();

	WeaponActionDataAssetTypeAction = MakeShared<FWeaponActionDataAssetTypeAction>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
}

void FDogFightEditorModule::ShutdownModule()
{
	FDogFightEditorCommands::Unregister();

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(WeaponActionDataAssetTypeAction.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDogFightEditorModule, DogFightEditor)