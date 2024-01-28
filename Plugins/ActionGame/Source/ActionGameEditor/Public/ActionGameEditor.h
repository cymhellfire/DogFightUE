#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAssetTypeActions_WeaponActionData;
class FAssetTypeActions_TargetingWeaponActionData;

class FActionGameEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FAssetTypeActions_WeaponActionData> WeaponActionDataAssetTypeAction;
	TSharedPtr<FAssetTypeActions_TargetingWeaponActionData> TargetingWeaponActionDataAssetTypeAction;
};
