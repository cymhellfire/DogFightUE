/* Copyright (C) 2021 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the UE4 Marketplace
*/

#include "ElectronicNodes.h"
#include "ENConnectionDrawingPolicy.h"
#include "ENCommands.h"
#include "NodeFactory.h"
#include "Interfaces/IPluginManager.h"
#include "Lib/HotPatch.h"
#include "MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Patch/NodeFactoryPatch.h"
#include "Popup/ENUpdatePopup.h"
#include "SettingsEditor/Public/ISettingsEditorModule.h"

#define LOCTEXT_NAMESPACE "FElectronicNodesModule"

void FElectronicNodesModule::StartupModule()
{
	const TSharedPtr<FENConnectionDrawingPolicyFactory> ENConnectionFactory = MakeShareable(new FENConnectionDrawingPolicyFactory);
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(ENConnectionFactory);

	auto const CommandBindings = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame").GetMainFrameCommandBindings();
	ENCommands::Register();

	CommandBindings->MapAction(
		ENCommands::Get().ToggleMasterActivation,
		FExecuteAction::CreateRaw(this, &FElectronicNodesModule::ToggleMasterActivation)
	);

	PluginDirectory = IPluginManager::Get().FindPlugin(TEXT("ElectronicNodes"))->GetBaseDir();
	GlobalSettingsFile = PluginDirectory + "/Settings.ini";

	ElectronicNodesSettings = GetMutableDefault<UElectronicNodesSettings>();
	ElectronicNodesSettings->OnSettingChanged().AddRaw(this, &FElectronicNodesModule::ReloadConfiguration);

	if (ElectronicNodesSettings->UseGlobalSettings)
	{
		if (FPaths::FileExists(GlobalSettingsFile))
		{
			ElectronicNodesSettings->LoadConfig(nullptr, *GlobalSettingsFile);
		}
	}

	if (ElectronicNodesSettings->UseHotPatch && ElectronicNodesSettings->MasterActivate)
	{
#if PLATFORM_WINDOWS && !UE_BUILD_SHIPPING
		FHotPatch::Hook(&FNodeFactory::CreateConnectionPolicy, &FNodeFactoryPatch::CreateConnectionPolicy_Hook);
#endif
	}

	if (ElectronicNodesSettings->ActivatePopupOnUpdate)
	{
		ENUpdatePopup::Register();
	}
}

#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 25
void FElectronicNodesModule::ReloadConfiguration(FName PropertyName)
#else
void FElectronicNodesModule::ReloadConfiguration(UObject* Object, struct FPropertyChangedEvent& Property)
#endif
{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25) || (ENGINE_MAJOR_VERSION == 5)
	const FName PropertyName = Property.GetPropertyName();
#endif

	if (PropertyName == "UseGlobalSettings")
	{
		if (ElectronicNodesSettings->UseGlobalSettings)
		{
			if (FPaths::FileExists(GlobalSettingsFile))
			{
				ElectronicNodesSettings->LoadConfig(nullptr, *GlobalSettingsFile);
			}
			else
			{
				ElectronicNodesSettings->SaveConfig(CPF_Config, *GlobalSettingsFile);
			}
		}
	}

	if (PropertyName == "UseHotPatch")
	{
		ISettingsEditorModule* SettingsEditorModule = FModuleManager::GetModulePtr<ISettingsEditorModule>("SettingsEditor");
		if (SettingsEditorModule)
		{
			SettingsEditorModule->OnApplicationRestartRequired();
		}
	}

	if (ElectronicNodesSettings->LoadGlobalSettings)
	{
		if (FPaths::FileExists(GlobalSettingsFile))
		{
			ElectronicNodesSettings->LoadConfig(nullptr, *GlobalSettingsFile);
		}
		ElectronicNodesSettings->LoadGlobalSettings = false;
	}

	ElectronicNodesSettings->SaveConfig();

	if (ElectronicNodesSettings->UseGlobalSettings)
	{
		ElectronicNodesSettings->SaveConfig(CPF_Config, *GlobalSettingsFile);
	}
}

void FElectronicNodesModule::ShutdownModule()
{
}

void FElectronicNodesModule::ToggleMasterActivation() const
{
	ElectronicNodesSettings->ToggleMasterActivation();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElectronicNodesModule, ElectronicNodes)
