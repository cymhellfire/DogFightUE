#pragma once

#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCustomizableCard, Log, All);

class FCustomizableCardModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};