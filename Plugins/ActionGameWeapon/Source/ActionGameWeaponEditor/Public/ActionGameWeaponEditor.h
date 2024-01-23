#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FWeaponActionDataAssetTypeAction;

class FActionGameWeaponEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FWeaponActionDataAssetTypeAction> WeaponActionDataAssetTypeAction;
};
