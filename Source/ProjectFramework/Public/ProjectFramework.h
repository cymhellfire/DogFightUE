#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Net/UnrealNetwork.h"
#include "Online.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectFramework, Log, All);

class FProjectFrameworkModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
