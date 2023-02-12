#include "DogFightEditor.h"

#include "DogFightEditorCommands.h"
#include "DogFightEditorStyle.h"
#include "AssetFactory/AssetType/WarheadAssetTypeActions.h"
#include "Toolbars/MainWindowToolbar.h"

#define LOCTEXT_NAMESPACE "FDogFightEditorModule"

void FDogFightEditorModule::StartupModule()
{
	Style = FDogFightEditorStyle::GetInstance();

	FDogFightEditorCommands::Register();

	MainWindowToolbar = MakeShareable(new FMainWindowToolbar);
	MainWindowToolbar->Initialize();

	WarheadAssetTypeActions = MakeShared<FWarheadAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(WarheadAssetTypeActions.ToSharedRef());
}

void FDogFightEditorModule::ShutdownModule()
{
	FDogFightEditorCommands::Unregister();

	if (!FModuleManager::Get().IsModuleLoaded("AssetTools"))
		return;

	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(WarheadAssetTypeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDogFightEditorModule, DogFightEditor)