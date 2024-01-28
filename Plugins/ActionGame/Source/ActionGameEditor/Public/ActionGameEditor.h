#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FWeaponActionDataAssetTypeAction;

class FActionGameEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FWeaponActionDataAssetTypeAction> WeaponActionDataAssetTypeAction;
};
