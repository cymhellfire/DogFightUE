﻿#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

ATTRIBUTESYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogAttributeSystem, Log, All);

class FAttributeSystemModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
