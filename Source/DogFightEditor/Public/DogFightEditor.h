#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMainWindowToolbar;

class FDogFightEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TSharedPtr<ISlateStyle> Style;
    TSharedPtr<FMainWindowToolbar> MainWindowToolbar;
};
