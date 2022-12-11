#include "CreateLuaScriptWindow.h"

#include "SCreateLuaScriptWindow.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE		"CreateScriptWindow"

void FCreateLuaScriptWindow::OpenCreateLuaScriptWindow()
{
	const FText WindowTitle = LOCTEXT("WindowTitle", "Create Lua Script");

	TSharedPtr<SWindow> CreateScriptWindow = SNew(SWindow)
		.Title(WindowTitle)
		.ClientSize(FVector2D(600.f, 240.f))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.SizingRule(ESizingRule::FixedSize)
		[
			SNew(SCreateLuaScriptWindow)
		];

	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	TSharedPtr<SWindow> ParentWindow = MainFrame.GetParentWindow();

	if (ParentWindow.IsValid())
	{
		FSlateApplication::Get().AddModalWindow(CreateScriptWindow.ToSharedRef(), ParentWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(CreateScriptWindow.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE
