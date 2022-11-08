#include "DogFightEditorCommands.h"

#define LOCTEXT_NAMESPACE "FDogFightEditorCommands"

void FDogFightEditorCommands::RegisterCommands()
{
	UI_COMMAND(CreateLuaScript, "Create Lua Script", "Create a lua script.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
