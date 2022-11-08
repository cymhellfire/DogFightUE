#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/UICommandList.h"

class FMainWindowToolbar
{
public:
	FMainWindowToolbar();

	void Initialize();

	TSharedRef<SWidget> GenerateMainMenu();

protected:
	const TSharedRef<FUICommandList> CommandList;
};
