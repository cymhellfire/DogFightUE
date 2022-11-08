#pragma once

#include "Framework/Commands/Commands.h"

class FDogFightEditorCommands : public TCommands<FDogFightEditorCommands>
{
public:
	FDogFightEditorCommands()
		: TCommands<FDogFightEditorCommands>(TEXT("DogFightEditor"), NSLOCTEXT("Contexts", "DogFightEditor", "DogFight Editor"), NAME_None, "DogFightEditorStyle")
	{}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> CreateLuaScript;
};
