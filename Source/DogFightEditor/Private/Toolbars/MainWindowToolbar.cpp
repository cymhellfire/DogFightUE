#include "MainWindowToolbar.h"
#include "DogFightEditorCommands.h"
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "Utils/DogFightUtilsFunctionLibrary.h"
#include "Windows/CreateLuaScriptWindow.h"

#define LOCTEXT_NAMESPACE	"DogFightMainMenu"

FMainWindowToolbar::FMainWindowToolbar()
	: CommandList(new FUICommandList)
{
	CommandList->MapAction(FDogFightEditorCommands::Get().CreateLuaScript, FExecuteAction::CreateStatic(FCreateLuaScriptWindow::OpenCreateLuaScriptWindow));
}

void FMainWindowToolbar::Initialize()
{
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
	FToolMenuSection& Section = ToolbarMenu->AddSection("DogFightMenu");
	Section.AddEntry(FToolMenuEntry::InitComboButton(
		"DogFightMenu",
		FUIAction(),
		FOnGetContent::CreateRaw(this, &FMainWindowToolbar::GenerateMainMenu),
		LOCTEXT("DogFightTitle", "DogFight"),
		LOCTEXT("DogFightTooltip", "DogFight Menu"),
		FSlateIcon("DogFightEditorStyle", "DogFightEditor.DogFightLogo")));
}

TSharedRef<SWidget> FMainWindowToolbar::GenerateMainMenu()
{
	const FDogFightEditorCommands& Commands = FDogFightEditorCommands::Get();
	FMenuBuilder MenuBuilder(true, CommandList);

	MenuBuilder.BeginSection(NAME_None, LOCTEXT("Section_Script", "Script"));
	MenuBuilder.AddMenuEntry(Commands.CreateLuaScript, NAME_None, LOCTEXT("CreateScript", "Create Script"));
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE
