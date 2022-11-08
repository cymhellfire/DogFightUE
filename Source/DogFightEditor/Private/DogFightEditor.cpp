#include "DogFightEditor.h"

#include "DogFightEditorCommands.h"
#include "DogFightEditorStyle.h"
#include "Toolbars/MainWindowToolbar.h"

#define LOCTEXT_NAMESPACE "FDogFightEditorModule"

void FDogFightEditorModule::StartupModule()
{
	Style = FDogFightEditorStyle::GetInstance();

	FDogFightEditorCommands::Register();

	MainWindowToolbar = MakeShareable(new FMainWindowToolbar);
	MainWindowToolbar->Initialize();
}

void FDogFightEditorModule::ShutdownModule()
{
	FDogFightEditorCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDogFightEditorModule, DogFightEditor)