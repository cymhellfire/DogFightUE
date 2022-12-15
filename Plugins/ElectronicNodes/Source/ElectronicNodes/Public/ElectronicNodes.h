/* Copyright (C) 2021 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the UE4 Marketplace
*/

#pragma once

#include "CoreMinimal.h"

#include "ElectronicNodesSettings.h"
#include "Modules/ModuleInterface.h"

class FElectronicNodesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void ToggleMasterActivation() const;

private:
	UElectronicNodesSettings* ElectronicNodesSettings = nullptr;
	FString PluginDirectory;
	FString GlobalSettingsFile;

#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 25
	void ReloadConfiguration(FName PropertyName);
#else
	void ReloadConfiguration(UObject* Object, struct FPropertyChangedEvent& Property);
#endif
};
